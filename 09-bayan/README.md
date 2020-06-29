# 09-bayan

Finds files-duplicates.

## Allowed options:

- size - Block size (in bytes) used to compare files (at least 1, 4096 by default)
- hash - Hash algorithm used to compare byte blocks, one of 'crc32', 'crc16' (crc32 by default)
- min_file_size - Minimum file size (in bytes) to compare
- root_dir - Directories to search duplicates into
- exclude_dir - Direcroties to exclude from search
- mask_include - Include only files corresponding to these masks in search
- mask_exclude arg - Exclude files corresponding to these masks from search
- recursive - Use this option to enable recursive subdirectory scanning
- case_sensitive - Use this option to make file masks case sensitive

## Examples

Assuming the content of current directory is:

- a/
  - 1.log
  - 2.LOG
  - b/
    - 3.log
    - 4.in
  - c/
    - 5.in
    - 6.in
    
Files content:

1.log, 2.LOG, 5.in:

```
abc
```

3.log, 6.in:

```
Hello, world!
```

4.in:

```
xyz
```

Output examples:

```
$ ./09-bayan --root_dir a
"a/1.log"
"a/2.LOG"

$ ./09-bayan --root_dir a --recursive
"a/b/3.log"
"a/c/6.in"

"a/1.log"
"a/2.LOG"
"a/c/5.in"

$ ./09-bayan --root_dir a --recursive --exclude_dir a/c
"a/1.log"
"a/2.LOG"

$ ./09-bayan --root_dir a a/c
"a/c/5.in"
"a/1.log"
"a/2.LOG"

$ ./09-bayan --root_dir a --recursive --mask_include .*\.log
"a/1.log"
"a/2.LOG"

$ ./09-bayan --root_dir a --recursive --mask_exclude .*\.log --case_sensitive
"a/2.LOG"
"a/c/5.in"

$ ./09-bayan --root_dir a --recursive --min_file_size 4 --size 1
"a/b/3.log"
"a/c/6.in"
```
