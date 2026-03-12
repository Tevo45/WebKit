;; Per-directory local variables for GNU Emacs 23 and later.

;; Syntax: ((MODE (VAR . VAL) ...) ...)
;; MODE is a symbol like `c-mode', or `nil' for all modes.
((nil
  (eglot-server-programs
   ((c-mode c-ts-mode c++-mode c++-ts-mode)
    "~/var/src/webkit/webkit-clangd" "--enable-config" "--limit-results=8192")))
 (c-mode
  (indent-tabs-mode . nil)
  (c-basic-offset . 4))
 (c++-mode
  (indent-tabs-mode . nil)
  (c-basic-offset . 4))
 (java-mode
  (indent-tabs-mode . nil)
  (c-basic-offset . 4))
 (ruby-mode
  (ruby-indent-level . 4))
 (change-log-mode
  (indent-tabs-mode . nil)))
