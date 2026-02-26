class sum {
  public:
      int d(int* a, int size) {
          int s = 0;
          for (int i = 0; i < size; i++)
              s += a[i];
          return s;
      }
};
