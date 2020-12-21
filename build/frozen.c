#include <stdint.h>
const char mp_frozen_str_names[] = {
"site.py\0"
"imp_pivot.py\0"
"imp.py\0"
"types.py\0"
"imp_empty_pivot_module.py\0"
"\0"};
const uint32_t mp_frozen_str_sizes[] = {
147,
171,
2236,
541,
21,
};
const char mp_frozen_str_content[] = {
"import sys\nimport builtins\n\nfor name in (\"sys\", \"__main__\", \"builtins\"):\n    sys.modules[name] = __import__(name)\n\nimport imp\n\nbuiltins.imp = imp\n\n\0"
"__name__ = imp.pivot_name\n__file__ = imp.pivot_file\nprint('pivot',__name__,__file__, globals())\n\nexec( compile( imp.pivot_code, __file__, 'exec') , globals(), globals() )\n\0"
"import sys\nimport builtins\nimport types\n\n# keep the builtin function accessible in this module and via imp.__import__\n__import__ = __import__\n\n# Deprecated since version 3.4: Use types.ModuleType instead.\n# but micropython aims toward full 3.4\n\n# Return a new empty module object called name. This object is not inserted in sys.modules.\ndef new_module(name):\n    return types.ModuleType(name)\n\n\ndef importer(name, *argv, **kw):\n    global __import__\n    try:\n        return __import__(name, *argv)\n    except ImportError:\n        pass\n\n    filen = ':{0}.py'.format(name)\n    print(\"INFO: getting online local version of\", filen, file=sys.stderr)\n    # todo open the file via open() or raise importerror\n    try:\n        code = open(filen, 'r').read()\n    except:\n        remote = False\n        for i, path_url in enumerate(sys.path):\n            if path_url.startswith('http://') or path_url.startswith('https://'):\n                filen = '{0}/{1}.py'.format(path_url, name)\n                print(\"INFO: try to get online remote version of\", filen, file=sys.stderr)\n                try:\n                    code = open(filen, 'r').read()\n                    remote = True\n                    break\n                except:\n                    continue\n        else:\n            raise ImportError('module not found')\n\n    # build a empty module\n    mod = types.ModuleType(name)\n\n    mod.__file__ = filen\n\n    # compile module from cached file\n    try:\n        code = compile(code, filen, 'exec')\n    except Exception as e:\n        sys.print_exception(e)\n        raise\n\n    # micropython would insert module before executing the whole body\n    # and left it that way even on runtime error.\n    sys.modules[name] = mod\n\n    # execute it in its own empty namespace.\n    try:\n        ns = vars(mod)\n    except:\n        print(\"WARNING: this python implementation lacks vars()\", file=sys.stderr)\n        ns = mod.__dict__\n\n    try:\n        exec(code, ns, ns)\n    except Exception as e:\n        sys.print_exception(e)\n        # do not follow weird micropython behaviour and clean up zombie module\n        del sys.modules[name]\n        raise\n\n    return mod\n\n\n# install hook\nbuiltins.__import__ = importer\n# print(\"__import__ is now\", importer)\n\0"
"import sys\nimport imp\n\n\n# not thread safe\n\ndef ModuleType(name):\n    if sys.modules.get('name'):\n        print(\"Error : module %s exists !\"%name)\n        return sys.modules[name]\n\n    # get a new fresh module\n    # be sure to use the builtin func\n    pivot = imp.__import__('imp_empty_pivot_module') #\n    # low risk, who would call his module like that ?\n    del sys.modules['imp_empty_pivot_module']\n\n    #still unknown at this time\n    if hasattr(pivot,'__file__'):\n        del pivot.__file__\n\n    pivot.__name__ = name\n\n    return pivot\n\0"
"__dict__ = globals()\n\0"
};
