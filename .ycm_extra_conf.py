import ycm_core

flags = [
    '-Wall',
    '-Werror',
    '-std=c++17',
    '-x',
    'c++',
    '-I',
    'include'
]

def FlagsForFile(filename, **kwargs):
    return {
        'flags': flags,
        'do_cache': True
    }
