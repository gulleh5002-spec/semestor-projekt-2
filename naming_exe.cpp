class Sum 
{
  public: 
  int beregnSum(int* tal, int size) 
      {
          constexpr int sum = { 0 }; // Brug constexpr og const til lokale variabler
          for (int i = 0; i < size; i++)
          sum += tal[i];
          return sum;
      }
};



