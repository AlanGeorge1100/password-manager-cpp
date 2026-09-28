#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<iomanip>

void displayMessege(const std::string& messege){

    std:: cout << messege << std::endl;
}

void displayMenue(){
    displayMessege("\nPassword Manager");
    displayMessege(" 1. Add Password");
    displayMessege(" 2. Find Pasword");
    displayMessege(" 3. Exit");
    displayMessege("Enter your choice: ");
    
}

std::string xorCrypt(const std::string& text, const std::string& key) {
    std::string out = text;
    for (size_t i = 0; i < text.size(); ++i)
        out[i] = text[i] ^ key[i % key.size()];
    return out;
}

std::string toHex(const std::string& s) {
    std::ostringstream os;
    for (unsigned char c : s)
        os << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return os.str();
}

std::string fromHex(const std::string& hex) {
    std::string out;
    for (size_t i = 0; i + 1 < hex.size(); i += 2)
        out += (char)std::stoi(hex.substr(i, 2), nullptr, 16);
    return out;
}


void addPassword(std:: string &cite , std::string &username , std::string &password){
    std::ofstream file("Passwords.txt", std::ios::app);
    if(file.is_open()){
        file << cite << " " << username << " " << toHex(xorCrypt(password, "aubmeyang")) << "\n";
        file.close();
        displayMessege("Password saved succesfully");
    }else{
        std::cerr << "Error: Unable to open a file for writing.\n";
    }
}

void extract(std:: string &line , std:: string &cite, std::string &username , std::string &password){
    int left = 0 , right = 0;
    right = line.find_first_of(' ');
    cite = line.substr(left,right-left);

    left = right+1;
    right = line.find_first_of(' ',left);
    username = line.substr(left, right-left);

    left = right+1;
    password = line.substr(left,line.size()-left);
}

void findPassword(std::string &findCite){

    std::map<std::string,std::vector<std::pair<std::string,std::string>>>mp;
    std::ifstream file("Passwords.txt");
    if(!file.is_open()){
        std::cerr << "Error: can't find passwords.txt .\n";
        return;
    }

    std::string line = "",cite = "", username = "", password ="";
    while(getline(file, line)){
        extract(line , cite, username, password);
        mp[cite].push_back({username,password});
    }

    file.close();

    if(mp.count(findCite) == 0){
        displayMessege("Cite not Found");
        return;
    }

    std::cout <<"Found " << mp[findCite].size() << " username and Password for cite " << findCite << std::endl;
    for(auto &details : mp[findCite]){
        std::cout << "==> " <<  details.first << " " << xorCrypt(fromHex(details.second), "aubmeyang") << std::endl;
    }

    return;
}


int main(){


    int choice = 0;
    std::string cite = "", username = "" , password = "";

    do{

        displayMenue();
        std:: cin >> choice;

        if(choice == 1){
            std:: cout << "Enter cite : ";
            std:: cin >> cite;
            
            std:: cout << "Enter username : ";
            std:: cin >> username;
           
            std:: cout << "Enter password : ";
            std:: cin >> password;
        
            addPassword(cite, username , password);

        }else if(choice == 2){
            displayMessege("Enter cite");
            std::cin >> cite;
            findPassword(cite);

        }else{
            displayMessege("Exiting");
        }


    }while(choice != 3);

    

return 0;

}