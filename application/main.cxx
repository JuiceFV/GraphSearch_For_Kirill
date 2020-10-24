#include "WordSimilarity.h"

int main()
{ 
	setlocale(LC_ALL, "rus");
    std::cout << smlrt::findeSimilarWord({"Los Angeles", "New York", "Washington", "Texas"}, "Texds", 2, false);
	return (0);
}