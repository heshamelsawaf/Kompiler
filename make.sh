g++ *.cpp
./a.out > example.dot
dot -Tpng example.dot -o example.png
xdg-open example.png
