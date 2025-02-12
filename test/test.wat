;; (module
;;   (func (export "add") (param i32 i32 i32) (result i32)
;;     local.get 0
;;     local.get 1
;;     i32.add)
;; )

(module
  (func (export "add") (param i32 i32 i32) (result i32)
    local.get 0
    local.get 1
    i32.add    ;; Add first two parameters
    local.get 2
    i32.add    ;; Add the result to the third parameter
  )
)
