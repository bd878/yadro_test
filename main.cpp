#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char const *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " test_file.txt\n";
    return 2;
  }

  std::fstream fin(argv[1]);

  if (!fin.is_open()) {
    std::cerr << "failed to open " << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }

  int table_count, hour_price;
  std::string time_begin, time_end;
  fin >> table_count >> time_begin >> time_end >> hour_price;

  std::cout << "Table count : " << table_count << "\n"
    << "Hour price : " << hour_price << "\n"
    << "Time begin : " << time_begin << "\n"
    << "Time end : " << time_end << "\n";

  return 0;
}