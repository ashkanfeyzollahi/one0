## [0.1.0] - 2025-12-18

### 🚀 Features

- *(gc)* [**breaking**] Add utility functions and switch remove_ptr to index-based
- *(mem)* Add xstrdup and improve xfree/xrealloc for safe GC tracking

### 🐛 Bug Fixes

- Read source file line by line instead of tokenizing by '\n'
- *(utils)* Initialize variable in bin_to_ch to avoid undefined behavior

### 🚜 Refactor

- Modularize codebase and update build configuration
- *(tokenizer)* Replace strdup with xstrdup

### 📚 Documentation

- Update README to reflect line-by-line reading

### ⚙️ Miscellaneous Tasks

- Init
- *(gc)* Use size_t in collect_garbages loop and include stdint.h
- *(changelog)* Add git-cliff configuration
