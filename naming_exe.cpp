class sum 
{
  public: 
  int bergenSum(int* tal, int size) 
      {
          int sum = 0;
          for (int i = 0; i < size; i++)
          sum += tal[i];
          return sum;
      }
};



