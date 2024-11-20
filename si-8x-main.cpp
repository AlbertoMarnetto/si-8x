#include <iostream>
#include <fstream>
#include <optional>
#include <vector>

extern uint8_t  diff_stunt_v0_bin[];
extern unsigned diff_stunt_v0_bin_len;
extern uint8_t diff_makeone_v0_bin[];
extern unsigned diff_makeone_v0_bin_len;
extern uint8_t diff_playone_v0_bin[];
extern unsigned diff_playone_v0_bin_len;
extern uint8_t diff_stunt_v3_bin[];
extern unsigned diff_stunt_v3_bin_len;
extern uint8_t diff_makeone_v3_bin[];
extern unsigned diff_makeone_v3_bin_len;
extern uint8_t diff_playone_v3_bin[];
extern unsigned diff_playone_v3_bin_len;

std::vector<uint8_t> read_file(
    std::string const & path,
    std::string const & filename)
{
    std::string filepath = path + "/" + filename;
    std::ifstream ifstream(filepath, std::ios::binary);
    std::vector<uint8_t> buffer(
        (std::istreambuf_iterator<char>(ifstream)),
        std::istreambuf_iterator<char>());
    if (ifstream.fail() || buffer.empty())
    {
        std::cerr << "Unable to read " << filepath << "\n";
        exit(1);
    }
    return buffer;
};

void write_file(
    std::string const & path,
    std::string const & filename,
    std::vector<uint8_t> const & buffer)
{
    std::string filepath = path + "/" + filename;
    std::ofstream ofstream(filepath, std::ios::binary | std::ios::trunc);
    std::copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(ofstream));

    if (ofstream.fail()) {
        std::cerr << "Unable to write " << filepath << "\n";
        exit(1);
    }
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

    std::vector<uint8_t> stunt_buffer = read_file(path, "STUNT.EXE");
    std::vector<uint8_t> makeone_buffer = read_file(path, "MAKEONE.EXE");
    std::vector<uint8_t> playone_buffer = read_file(path, "PLAYONE.EXE");

    struct Patchset{
        std::string description;
        uint8_t const * stunt_diff;
        std::size_t stunt_diff_len;
        uint8_t const * makeone_diff;
        std::size_t makeone_diff_len;
        uint8_t const * playone_diff;
        std::size_t playone_diff_len;
    };
    std::vector<Patchset> patchsets =
    {
        {
            "Vanilla (e.g. GOG version)",
            diff_stunt_v0_bin,
            diff_stunt_v0_bin_len,
            diff_makeone_v0_bin,
            diff_makeone_v0_bin_len,
            diff_playone_v0_bin,
            diff_playone_v0_bin_len,
        },
        {
            "P3 - Final Patch Set from Stunt Island Harbor",
            diff_stunt_v3_bin,
            diff_stunt_v3_bin_len,
            diff_makeone_v3_bin,
            diff_makeone_v3_bin_len,
            diff_playone_v3_bin,
            diff_playone_v3_bin_len,
        },
    };

    bool patch_success = true;
    for (Patchset const & candidate_patchset : patchsets)
    {
        if (stunt_buffer.size() == candidate_patchset.stunt_diff_len
            && makeone_buffer.size() == candidate_patchset.makeone_diff_len
            && playone_buffer.size() == candidate_patchset.playone_diff_len
        )
        {
            std::cout << "Found version: " << candidate_patchset.description << "\n";
            for (size_t i = 0; i < stunt_buffer.size(); ++i) {
                stunt_buffer[i] ^= diff_stunt_v0_bin[i];
            }
            for (size_t i = 0; i < makeone_buffer.size(); ++i) {
                makeone_buffer[i] ^= diff_makeone_v0_bin[i];
            }
            for (size_t i = 0; i < playone_buffer.size(); ++i) {
                playone_buffer[i] ^= diff_playone_v0_bin[i];
            }

            std::cout << "Writing STUNT1.EXE, MAKEONE1.EXE and PLAYONE1.EXE with 8x detail\n";
            write_file(path, "STUNT1.EXE", stunt_buffer);
            write_file(path, "MAKEONE1.EXE", makeone_buffer);
            write_file(path, "PLAYONE1.EXE", playone_buffer);

            break;
        }
    }

    if (not patch_success)
    {
        std::cerr << "Failed to find a supported Stunt Island version in "
            << path << "\n";
        return 1;
    }

    return 0;
}
