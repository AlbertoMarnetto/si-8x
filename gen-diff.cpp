#include <iostream>
#include <fstream>
#include <vector>


int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <file1> <file2>" << std::endl;
        return 1;
    }

    std::ifstream file1(argv[1], std::ios::binary);
    std::ifstream file2(argv[2], std::ios::binary);
    std::ofstream diffFile("diff.bin", std::ios::binary);

    if (!file1.is_open() || !file2.is_open() || !diffFile.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return 1;
    }

    std::vector<char> buffer1((std::istreambuf_iterator<char>(file1)), std::istreambuf_iterator<char>());
    std::vector<char> buffer2((std::istreambuf_iterator<char>(file2)), std::istreambuf_iterator<char>());

    size_t size1 = buffer1.size();
    size_t size2 = buffer2.size();
    size_t minSize = std::min(size1, size2);

    if (size1 != size2) {
        std::cerr << "File size warning: " << size1 << " neq " << size2 << std::endl;
    }

    std::vector<char> diffBuffer(minSize);
    for (size_t i = 0; i < minSize; ++i) {
        diffBuffer[i] = buffer1[i] ^ buffer2[i];
    }

    diffFile.write(diffBuffer.data(), diffBuffer.size());

    file1.close();
    file2.close();
    diffFile.close();

    std::cout << "XOR difference written to diff.bin" << std::endl;

    return 0;
}
