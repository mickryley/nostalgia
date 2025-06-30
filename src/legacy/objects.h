struct Obj_Vector2 {
    int x, y;
    Obj_Vector2(int a, int b) : x(a), y(b) {
       // std::cout << "Obj_Vector2(" << x << ", " << y << ")\n";
    }
    ~Obj_Vector2() {
      //  std::cout << "~Obj_Vector2(" << x << ", " << y << ")\n";
    }
};