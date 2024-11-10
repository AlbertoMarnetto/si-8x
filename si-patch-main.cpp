#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

extern uint8_t  diff_stunt_v0_bin[];
extern unsigned diff_stunt_v0_bin_len;
extern uint8_t diff_makeone_v0_bin[];
extern unsigned diff_makeone_v0_bin_len;
extern uint8_t diff_stunt_v3_bin[];
extern unsigned diff_stunt_v3_bin_len;
extern uint8_t diff_makeone_v3_bin[];
extern unsigned diff_makeone_v3_bin_len;

void write_file(
    std::vector<uint8_t> const & buffer,
    std::string const & filename)
{
}

int main(int argc, char** argv)
{
    std::string path = ".";
    if (argc == 2)
    {
        path = std::string(argv[1]);
    }
    else if (argc > 1)
    {
        std::cerr << "Usage: " << argv[0] << " [$SI_PATH]\n\n"
            "Where $SI_PATH is the path of your Stunt Island installation";
        return 1;
    }

    auto read_file = [&](std::string const & filename) -> std::vector<uint8_t>
    {
        std::cerr << __FILE__ << " : " << __LINE__ << " : "  << __PRETTY_FUNCTION__ << std::endl;
        std::string filepath = path + "/" + filename;
        std::ifstream ifstream(filepath, std::ios::binary);
        std::vector<uint8_t> buffer(
            (std::istreambuf_iterator<char>(ifstream)),
            std::istreambuf_iterator<char>());
        if (ifstream.bad() || buffer.empty())
        {
            std::cerr << "Unable to read " << filepath;
            exit(1);
        }
        return buffer;
    };

    std::vector<uint8_t> stunt_buffer = read_file("STUNT.EXE");
    std::vector<uint8_t> makeone_buffer = read_file("MAKEONE.EXE");

    struct Patchset{
        std::string description;
        uint8_t const * stunt_diff;
        std::size_t stunt_diff_len;
        uint8_t const * makeone_diff;
        std::size_t makeone_diff_len;
    };
    std::vector<Patchset> patchsets =
    {
        { "GOG-style",                                      diff_stunt_v0_bin, diff_stunt_v0_bin_len, diff_makeone_v0_bin, diff_makeone_v0_bin_len},
        { "P3 - Final Patch Set from Stunt Island Harbor",  diff_stunt_v3_bin, diff_stunt_v3_bin_len, diff_makeone_v3_bin, diff_makeone_v3_bin_len},
    };

    bool patch_success = true;
    for (Patchset const & candidate_patchset : patchsets)
    {
        std::cerr << "stunt_buffer.size() = " << stunt_buffer.size() << "\n";
        std::cerr << "makeone_buffer.size() = " << makeone_buffer.size() << "\n";

        if (stunt_buffer.size() == candidate_patchset.stunt_diff_len
            && makeone_buffer.size() == candidate_patchset.makeone_diff_len)
        {
        std::cerr << __FILE__ << " : " << __LINE__ << " : "  << __PRETTY_FUNCTION__ << std::endl;
            std::cout << "Found version: " << candidate_patchset.description << "\n";
            for (size_t i = 0; i < stunt_buffer.size(); ++i) {
                std::cerr << "i = " << i << "\n";

                stunt_buffer[i] ^= diff_stunt_v0_bin[i];
            }
            for (size_t i = 0; i < makeone_buffer.size(); ++i) {
                std::cerr << "i = " << i << "\n";
                makeone_buffer[i] ^= diff_makeone_v0_bin[i];
            }

            write_file(stunt_buffer, "STUNT1.EXE");
            write_file(makeone_buffer, "MAKEONE1.EXE");

            break;
        }
    }

    if (not patch_success)
    {
    }

    return 0;
}
