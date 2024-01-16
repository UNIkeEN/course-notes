#include<bits/stdc++.h>

using namespace std;

bool searchFile(const vector<string>& files, const string& target) {
    return find(files.begin(), files.end(), target) != files.end();
}

void copy(vector<string>& files, const string& target) {
    string path, fileName, ext, tmp;
    int i;

    size_t pos = target.find_last_of('/');
    path = (pos == string::npos) ? "" : target.substr(0, pos + 1);
    tmp = (pos == string::npos) ? target : tmp = target.substr(pos + 1);

    size_t pos2 = tmp.find_last_of('.');
    fileName = (pos2 == string::npos) ? tmp : tmp.substr(0, pos2);
    ext = (pos2 == string::npos) ? "" : tmp.substr(pos2);

    size_t pos3 = fileName.find_last_of('_');
    if (pos3 == string::npos || fileName.substr(pos+1, 2) != "副本"){
        tmp = fileName + "_副本";
        if (searchFile(files, path+tmp+ext)) {
            for(int i=1; i<INT_MAX; i++){
                if (!searchFile(files, path+tmp+to_string(i)+ext))
                    cout<<path+tmp+to_string(i)+ext<<endl;
                    files.push_back(path+tmp+to_string(i)+ext);
                    break;
            }
        } else {
            cout<<path+tmp+ext<<endl;
            files.push_back(path+tmp+ext);
        }
    }
    return;
}

int main(){
    
    string input;
    vector<string> files;

    while (getline(std::cin, input)) {
        if (input.empty()) {
            break;
        }
        istringstream iss(input);
        string file;
        while (iss >> file) {
            files.push_back(file);
        }
    }

    while (getline(cin, input)) {
        if (input.empty()) {
            break;
        }
        istringstream iss(input);
        string file_to_copy;
        while (iss >> file_to_copy) {
            if (searchFile(files, file_to_copy)) copy(files, file_to_copy);    
        }
    }

    sort(files.begin(), files.end());
    for (const auto& file : files) {
        cout << file << ' ';
    }
    cout << endl;

    return 0;

}