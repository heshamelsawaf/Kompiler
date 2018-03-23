make
./Kompiler > example.dot
dot -Tpng example.dot -o example.png
xdg-open example.png
