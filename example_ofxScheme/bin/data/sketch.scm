(define (render n)
  (cond
    ((> n 0)
    (draw-circle (* (OUTPUT_WIDTH) (cos (/ (time) (* 1000 n)))) (/ (OUTPUT_HEIGHT) 2) (loop n 0 (* (random n) n) 1) 6)
    (render (- n 1)))))

(background-alpha 0.04 0.71 0.9 0.01)
(set-color 0.0 0.1 0.15 0.3)

(noFill)
(render 50)

(push)
(translate (/ (OUTPUT_WIDTH) 2) (/ (OUTPUT_HEIGHT) 2) 0)
(rotate 0 (* 30 (sin (/ (time) 1000))) (* 120 (cos (/ (time) 1000))))
(draw-cube (* (OUTPUT_HEIGHT) (sin (/ (time) 10000))))
(pop)

;(draw-circle (random (OUTPUT_WIDTH)) (random (OUTPUT_HEIGHT)) (loop 0 0 200 3) 6)
;(draw-circle (MOUSE_X) (MOUSE_Y) (loop 1 0 200 3) 6)
