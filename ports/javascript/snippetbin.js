
var editor_cmd=0;

var snippetbin_url = "https://snippet-bin.herokuapp.com";

const query_revision = "script_direct";

function set_dirty(){
  document.querySelectorAll('.dirty_watcher').
    forEach(e => e.classList.add('dirty'));
  let link = document.getElementById("link");
  link.textContent = "Link to online script (you have unsaved changes)";
}

function clear_dirty(){
  document.querySelectorAll('.dirty_watcher.dirty').
    forEach(e => e.classList.remove('dirty'));
  link.textContent = "Link to online script";
}

var cache = {};
function cached_get(url){  
  if (url in cache) return Promise.resolve(cache[url]);
  else {
    return axios.get(url).then(res=>{
      cache[url] = res;
      return res;
    });
  }
}

function load_revision(revision, update_history){
  if (!revision) return;
  cached_get(snippetbin_url + '/load_file/'+revision).then(res=>{
    current_revision = revision;
    editor_cmd++;
    editor.setValue(res.data.text); 
    if (update_history){
      let history = document.getElementById("history");
      let revision_history_data = document.getElementById("history_data");
      while(revision_history_data.firstChild) {
        revision_history_data.removeChild(revision_history_data.firstChild);
      }
      res.data.history.forEach((item, i) => {
        let option = document.createElement('option');
        option.value = i;
        option.dataset.value = item;
        revision_history_data.appendChild(option);
      });
      history.max = res.data.history.length-1;
      history.value = 0;
    }
    let link = document.getElementById("link");
    link.setAttribute("href", update_query_string(window.location.href, query_revision, current_revision));
    link.textContent = "Link to online script";
    clear_dirty();
  }).catch(err=>{
    console.log(err);
  });
}

function save(event){
  event.target.disabled = true;
  axios.post(snippetbin_url + '/save_file', {
    "text": editor.getValue(),
    "original_revision": current_revision
  }).then(res=>{
    load_revision(res.data.revision, true);
    event.target.disabled = false;
  }).catch(err=>{
    console.log('ERROR: ' + err.message);
    event.target.disabled = false;
  });
}

function copy(){
  document.querySelectorAll('.copying_watcher').
    forEach(e => e.classList.add('copying'));

  let link = document.getElementById("link");
  link.select();
  document.execCommand("copy");
}

function copy_complete(){
  document.querySelectorAll('.copying_watcher').
    forEach(e => e.classList.remove('copying'));
}

