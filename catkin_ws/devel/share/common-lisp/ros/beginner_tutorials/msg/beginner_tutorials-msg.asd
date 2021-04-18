
(cl:in-package :asdf)

(defsystem "beginner_tutorials-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "valueMatrix" :depends-on ("_package_valueMatrix"))
    (:file "_package_valueMatrix" :depends-on ("_package"))
  ))