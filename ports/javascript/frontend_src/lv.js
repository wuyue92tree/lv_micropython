
var external_link = null;
var external_script_value = null;
var editor, iframe, term;
var script_passed = false;
var current_revision = null;
// Ctrl+L is mandatory ! need xterm.js 3.14+
function xterm_helper(term, key) {
    function ESC(data) {
        return String.fromCharCode(27)+data
    }
    if ( key.charCodeAt(0)==12 ) {
        var cy = 0+term.buffer.cursorY
        if ( cy > 0) {
        if (cy <= term.rows) {
            term.write( ESC("[B") )
            term.write( ESC("[J") )
            term.write( ESC("[A") )
        }

        term.write( ESC("[A") )
        term.write( ESC("[K") )
        term.write( ESC("[1J"))

        for (var i=1;i<cy;i++) {
            term.write( ESC("[A") )
            term.write( ESC("[M") )
        }
        term.write( ESC("[M") )
        }
        return false
    }
    return true
}
function forward_event(event) {
    if(iframe !== undefined && iframe !== null) {
        if(iframe.contentWindow !== null)
            iframe.contentWindow.document.dispatchEvent(new MouseEvent('mouseup'));
    }
}
/* From https://stackoverflow.com/a/1634841 */
function removeURLParameter(url, parameter) {
    //prefer to use l.search if you have a location/link object
    var urlparts = url.split('?');   
    if (urlparts.length >= 2) {

        var prefix = encodeURIComponent(parameter) + '=';
        var pars = urlparts[1].split(/[&;]/g);

        //reverse iteration as may be destructive
        for (var i = pars.length; i-- > 0;) {    
            //idiom for string.startsWith
            if (pars[i].lastIndexOf(prefix, 0) !== -1) {  
                pars.splice(i, 1);
            }
        }

        return urlparts[0] + (pars.length > 0 ? '?' + pars.join('&') : '');
    }
    return url;
}
document.onmouseup = forward_event;
function editor_change() {
}


/* MONACO COMPAT */
function getEditorValue() {
    return "editor value";
}
function setEditorValue(val) {
    
}
/* END MONACO COMPAT */
function runScript() {
    var $this = $("#run-button");
    $this.prop("disabled", "disabled");
    term.write('\x1bc');
    

    const context = document.getElementById("canvas").getContext('2d');

    context.clearRect(0, 0, canvas.width, canvas.height);


    iframe.setAttribute("data-cscript", LZString.compressToEncodedURIComponent(getEditorValue()));

    clear_iframe(iframe);
    iframe.src = get_iframe_url() /*+ "&timestamp=" + new Date().getTime()*/;
    iframe.contentWindow.location.href = iframe.src;
    console.log("Iframe src: " + iframe.src);
    term.setOption('disableStdin', true);
    term.write('Loading...\r');
}
function reenableButton() {
    $("#run-button").removeProp('disabled');
    if(term != undefined && term != null)
        term.setOption('disableStdin', false);
}
function get_iframe_url() {
    /* Assemble the URL */
    var num_url_chars = (window.location.href.indexOf('?'));
    var base_url = window.location.href.substr(0, (num_url_chars == -1) ? undefined : num_url_chars);
    var newPathname = base_url.substr(0, base_url.lastIndexOf('/'));
    newPathname += "/lvgl.html" /*+ '?' + "env=dev"*/;
    return newPathname;
}
function processScriptArg(url, lzstring){
    var script_passed_handler = function() {
        console.log("Script passed: " + script_passed);
        runScript();
        script_passed = false;
    };
    if(!lzstring) {
        // if url is a single url we convert it to array
        if(!Array.isArray(url)){
            url = [{"url": url}];
        }
        var prev_external_link = external_link;
        var request = null;

        var error_handler = function() {
            if(prev_external_link != null) {
                external_link = prev_external_link;
            }
            if(request != null)
                alert("Failed to load script due to error " + request.status + ": " + request.statusText + "\nThe contents of the external link box have been reverted.");
            else
                alert("The URL you passed is invalid.");
        };

        try {
            url.forEach(e=>{
                new URL(e.url);
            });
        } catch(e) {
            error_handler();
            return;
        }

        // last url is probably the important one
        external_link = url[url.length-1].url;
        // read text from URL location
        url.forEach(e=>{
            let request = new XMLHttpRequest();
            console.log("GET " + e.url);
            request.overrideMimeType("text/plain");
            request.open('GET', e.url, true);
            request.send(null);
            request.onerror = error_handler;
            request.onreadystatechange = function () {
                if (request.readyState === 4) {
                    if(request.status == 200) {
                        console.log(request.reponseText);
                        if(request.responseText === undefined)
                            return;
                        e.content = request.responseText;
                        // check if all of them have content
                        total = url.reduce((total, el) => total+("content" in el) ,0);
                        console.log(total+" of "+url.length+" loaded");
                        if(total==url.length){
                            final_script = "";
                            url.forEach(e=>{
                                if("comment" in e){
                                    final_script += "##### "+e.comment+" #####\n\n";
                                }
                                final_script += e.content + "\n\n";
                            })
                            setEditorValue(final_script);
                            script_passed_handler();
                        }
                    } else {
                        error_handler();
                    }
                }
            }
        });
    } else {
        external_link = null;
        // decompress LZString
        current_revision = url;
        load_revision(current_revision, true, function() {
            script_passed_handler();
        });
    }
    
}
function getSearchArg(argname) {
    /* Run custom script if passed */
    var custom = undefined;
    try {
        custom = new URL(window.location.href).searchParams.get(argname);
    } catch (e) {
        console.log(e + ": URL seems to be unsupported");
    }
    return custom;
}
function clear_iframe(iframe) {
    iframe.contentWindow.document.open();
    iframe.contentWindow.document.write("");
    iframe.contentWindow.document.close();
}

function setupPythonEditor() {
    const editorDiv = document.getElementById('editor');
    editorDiv.textContent = '';
    
    monaco.editor.create(editorDiv, {
        value: 'print("hello world")',
        language: 'python',
        automaticLayout: true // the important part
    });
}
$(window).load(function() {
    $(document).on('shown.bs.tooltip', function (e) {
        setTimeout(function () {
            $(e.target).tooltip('hide');
        }, 5000);
    });
    clear_dirty();
    /* Enable tooltips */
    $('[data-toggle="tooltip"]').tooltip(); 

    setupPythonEditor();
    /*
    editor = ace.edit("editor");
    editor.getSession().setUseWrapMode(true);
    editor.setAutoScrollEditorIntoView(true);
    var PythonMode = ace.require("ace/mode/python").Mode;
    editor.session.setMode(new PythonMode());
    */
    iframe = document.getElementById("emscripten-iframe");
    iframe.src = "about:blank";
    iframe.contentWindow.location.href = iframe.src;
    clear_iframe(iframe);
    

    
    Terminal.applyAddon(fit);
    term = new Terminal({
        tabStopWidth : 8,
            cursorBlink : true,
            cursorStyle : 'block',
            applicationCursor : true
    });
    mp_js_stdout = document.getElementById('mp_js_stdout');
    mp_js_stdout.value = "";
    term.open(mp_js_stdout);
    term.fit();
    term.on('data', function(key, e) {
        if ( xterm_helper(term, key) ) {
            for(var i = 0; i < key.length; i++) {
                if(iframe.contentWindow !== null)
                    iframe.contentWindow.mp_js_process_char(key.charCodeAt(i));
            }
        }
    });
    
    mp_js_stdout.addEventListener('print', function(e) {
        var text = e.data;
                            term.write(text);
    }, false);
    /*
    editor.getSession().on('change', editor_change);
    editor.on("input", function(e) {
        if (editor_cmd) editor_cmd--;
        else set_dirty();
    });
    editor.resize();
    */
    term.fit();
    var script = getSearchArg("script");
    var script_direct = getSearchArg("script_direct");
    var script_startup = getSearchArg("script_startup");
    if(script_direct !== undefined && script_direct !== null) {
        script_passed = true;
        processScriptArg(script_direct, true);
    } else if(script_startup !== undefined && script_startup !== null) {
        // both startup script and normal script are passed
        if(script !== undefined && script !== null) {
            script_passed = true;
            // pass an array of objects
            processScriptArg([
                {
                    "comment": "startup script",
                    "url": script_startup
                },
                {
                    "comment": "main script",
                    "url": script
                }
            ]);
        }else{
            script_passed = true;
            processScriptArg(script_startup);
        }
    } else if(script !== undefined && script !== null) {
        script_passed = true;
        processScriptArg(script);
    } else
        runScript();
});
$(window).resize(function() {
    //editor.resize();
    term.fit();
});