string read_file_str(string filename) {
    fstream f(filename, fstream::in);
    string s;
    getline(f, s, '\0');
    f.close();
    return s;
}