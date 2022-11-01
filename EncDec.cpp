#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <algorithm>


using namespace std;


bool isalnum(std :: string str) {
    for (int i = 0; i < str.length(); i++) {
        if (!(isalpha(str[i])) || !(isdigit(str[i]))) {
            return false;
        }
    }
    return true;
}

bool findElement(vector<int> vect, int elt) {
    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] == elt) {
            return true;
        }
    }
    return false;
}

vector<int> removeDuplicates(vector<int> orig) {
    vector<int> updatedvector;
    for (int i = 0; i < orig.size(); i++) {
        if (!(findElement(updatedvector, orig[i]))) {
            updatedvector.push_back(orig[i]);
        }
    }
    return updatedvector;
}


class TextCrypt {

protected:
    std :: string text;
    vector<int> leftindices;
    std :: string priv_key = "", onlyalpha = "", final_key = "", final_keyset = "", leftletters = "", keypart = "", letterpart = "";
    
public:
    void setText(std :: string text_in) {
        text = text_in;
    }

    //friend functions;
    friend bool findElement(vector<int> vect, int elt);
    friend vector<int> removeDuplicates(vector<int> orig);
    
    //resolution
    void leftLetters(vector<int> leftindices, std :: string text);


    void privKeyMaker(std :: string pub_key) {
        // rot pub_key first letter 1-26
        int rot_var = isdigit(pub_key[0]) ? pub_key[0] - 48 : (isupper(pub_key[0]) ? pub_key[0] - 64 : pub_key[0] - 96);
        // std :: string priv_key = "";
        for (int i = 0; i < pub_key.length(); i++) {
            if (isalpha(pub_key[i])) {
                if (isupper(pub_key[i])) {
                    priv_key += (pub_key[i] + rot_var <= 90) ? pub_key[i] + rot_var : 64 + (pub_key[i] + rot_var - 90);
                }
                else if (islower(pub_key[i])) {
                    priv_key += (pub_key[i] + rot_var <= 90) ? pub_key[i] + rot_var : 64 + (pub_key[i] + rot_var - 90);
                }
            }
            else if (isdigit(pub_key[i])) {
                priv_key += pub_key[i] + 3 <= 57 ? pub_key[i] + 3 : pub_key[i] - 7; // -10 + 3
            }
        }
        // return priv_key;
    }

    void onlyAlpha(std :: string pub_key) {
        // std :: string alph = "";
        for (int i = 0; i < pub_key.length(); i++) {
            if (isalpha(pub_key[i])) {
                onlyalpha += pub_key[i];
            }
        }
        // return alph;
    }

    void indicesLeft(std :: string alph) {
        vector<int> a;
        // vector<int>::iterator it;
        for (int i = 0; i < alph.length(); i++) {
            if (isupper(alph[i])) {
                a.push_back(alph[i] - 64);
            }
            else if (islower(alph[i])) 
                a.push_back(alph[i] -96);
        }

        leftindices = removeDuplicates(a);  
    }

    void finalKeyMaker(std :: string pub_key) {
        final_key = pub_key + priv_key;
        // return pub_key + priv_key;
    }

    void advancedKeySetMaker(std :: string pub_key) {
        final_keyset = pub_key + "+" +leftletters;
    }

    std :: string advavncedKeySetGetter() {
        return final_keyset;
    }

    void keySeparator(string received_key) {
        int i = 0;
        while (received_key[i] != '+') {
            keypart += received_key[i];
            i++;
        }
        i++;
        while (i < received_key.length()) {
            letterpart += received_key[i];
        }
    }

};


void TextCrypt :: leftLetters(vector<int> leftindices, std :: string text) {
    for (int i = 0; i < leftindices.size(); i++) {
        leftletters += text[i];
    }
}


class Encryptor : public TextCrypt {

public:
    std :: string normalEncryption(std :: string text, std :: string pub_key) {
        privKeyMaker(pub_key);
        finalKeyMaker(pub_key);
        int fkey_len = final_key.length();
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == 32) {
                text[i] = 126;
            }
            else if (text[i] - final_key[i%fkey_len]/10 == 92) {
                text[i] = 125;
            }
            else {
                text[i] = text[i] - final_key[i%fkey_len]/10;
            }
        }
        return text;
    }

    std :: string advEncryption(std :: string text, std :: string pub_key) {
        privKeyMaker(pub_key);
        onlyAlpha(pub_key);
        indicesLeft(onlyalpha);
        finalKeyMaker(pub_key);
        leftLetters(leftindices, text);
        advancedKeySetMaker(pub_key);
        
        int fkey_len = final_key.length();
        for (int i = 0; i < text.length(); i++) {
            if (!(findElement(leftindices, i))) {
                if (text[i] == 32) {
                text[i] = 126;
                }
                else if (text[i] - final_key[i%fkey_len]/10 ==92) {
                    text[i] = 125;
                }
                // else if (text[i] - final_key[i%fkey_len]/10 == 34) {
                //     text[i] = 124;
                // }
                else {
                    text[i] = text[i] - final_key[i%fkey_len]/10;
                }
            }
        }
        return text;
    }

    ~Encryptor() {
        std :: cout << "jiji\n"; 
    }
};


class Decryptor : public TextCrypt {

public:
    std :: string normalDecryption(std :: string text, std :: string pub_key) {
        privKeyMaker(pub_key);
        finalKeyMaker(pub_key);
        int fkey_len = final_key.length();
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == 126) {
                text[i] = 32;
            }
            else if (text[i] == 125) {
                text[i] = 92 + final_key[i%fkey_len]/10;
            }
            else {
                text[i] = text[i] + final_key[i%fkey_len]/10;
            }
        }
        return text;
    }

    std :: string advDecryption(std :: string text, std :: string final_keyset) {
        keySeparator(final_keyset);
        std :: string pub_key = keypart;
        privKeyMaker(pub_key);
        onlyAlpha(pub_key);
        indicesLeft(onlyalpha);
        finalKeyMaker(pub_key);
        leftLetters(leftindices, text);

        if (leftletters == letterpart){
            int fkey_len = final_key.length();

            for (int i = 0; i < text.length(); i++) {
                if (!(findElement(leftindices, i))) {
                    if (text[i] == 126) {
                        text[i] = 32;
                    }
                    else if (text[i] == 125) {
                        text[i] = 92 + final_key[i%fkey_len]/10;
                    }
                    else if (text[i] == 124) {
                        text[i] = 34 + final_key[i%fkey_len]/10;
                    }
                    else {
                        text[i] = text[i] + final_key[i % fkey_len] / 10;
                    }
                }
            }
            return text;
        }
        else {
            return "This text was not encrypted using this passkey(alphanumeric, max size = 10).";
        }
    }
    ~Decryptor() {
        std :: cout << "hihi\n"; 
    }
};


std::string getOsName()
{
    #ifdef _WIN32
    return "Win32";
    #elif _WIN64
    return "Win64";
    #else
    return "Other";
    #endif
}


void cls() {
    if (getOsName() == "Win32" || getOsName() == "Win64") {
        system("cls");
    }
    else {
        system("clear");
    }
}


int main(void) {
    cls();

    // variables and objects for the classes
    std :: string text;
    std :: string pubkey;
    std :: string receivedkey;
    // TextCrypt en;
    
    std :: string encryptedText;
    std :: string decryptedText;

    std :: cout << std :: endl << std :: endl;

    
    int choice = 0;
    char ch = 'a';
    do {
        Encryptor en;
        Decryptor dc;
        std :: cout << "\n\t\t\t\t\t\t\t\t.___________________." << std :: endl;
        std :: cout << "\t\t\t\t\t\t\t\t| Encrypto/Decrypto |" << std :: endl;
        std :: cout << "\t\t\t\t\t\t\t\t|___________________|" << std :: endl;

        std :: cout << "\t1. Encrypt text" << "\n" << "\t2. Decrypt text" <<"\n\t3. Exit" << std :: endl;
        std :: cout << "\tEnter purpose : ";
        std :: cin >> choice;
        std :: cout << std :: endl;
        switch (choice) {
            case 1: 
                std :: cout << "\t\tEnter text : ";
                std :: getline(cin >> ws, text);
                
                en.setText(text);
                
                std :: cout << "\t\t\ta. Normal Encryption" << std :: endl;
                std :: cout << "\t\t\tb. Advanced Encryption" << std :: endl;
                std :: cout << "\n\t\t\tEnter Your Choice: ";
                std :: cin >> ch;

                switch(ch){
                    if (text.length() <= 20) {
                        ch = 'a';
                    }
                    case 'a':
                        while (true) {
                            std :: cout << "\t\t\t\tEnter passkey(alphanumeric, max size = 10): ";
                            std :: cin >> pubkey;
                            if (isalnum(pubkey) && pubkey.length() <= 10) {
                                break;
                            }
                        }
                        encryptedText = en.normalEncryption(text, pubkey);
                        std :: cout << std :: endl << std :: endl;
                        std :: cout << "\t" << encryptedText << "\n\n";

                        // body


                        std :: cout << "Press any key to continue: ";
                        getchar();getchar();
                        cls();
                        cls();
                        break;
                    case 'b':
                        while (true) {
                            std :: cout << "\t\t\t\tEnter passkey(alphanumeric, max size = 10): ";
                            std :: cin >> pubkey;
                            if (isalnum(pubkey) && pubkey.length() <= 10) {
                                break;
                            }
                        }
                        encryptedText = en.advEncryption(text, pubkey);
                        std :: cout << std :: endl << std :: endl;
                        std :: cout << "\t" << encryptedText << "\n\n";

                        // body
                        std :: cout << "\t\t\tpasskey " << en.advavncedKeySetGetter() << "\n\n";



                        std :: cout << "Press any key to continue: ";
                        getchar();getchar();
                        cls();
                        break;
                    
                    default:
                        std :: cout << "\t\t\t\t Invalid Choice!\n";
                        std :: cout << "Press any key to continue: ";
                        getchar();getchar();
                        cls();
                        break;
                }
                break;
            case 2:
                std :: cout << "\t\tEnter text : ";
                std :: cin >> text;
                std :: cout << "\t\t\ta. Normal Decryption" << std :: endl;
                std :: cout << "\t\t\tb. Advanced Decryption" << std :: endl;
                std :: cout << "\n\t\t\tEnter Your Choice: ";
                std :: cin >> ch;

                switch(ch){
                    if (text.length() <= 20) {
                        ch = 'a';
                    }
                    case 'a':
                        while (true) {
                            std :: cout << "\t\t\t\tEnter passkey(alphanumeric, max size = 10): ";
                            std :: cin >> pubkey;
                            if (isalnum(pubkey) && pubkey.length() <= 10) {
                                break;
                            }
                        }
                        decryptedText = dc.normalDecryption(text, pubkey);
                        std :: cout << decryptedText << "\n\n";
                        // body



                        std :: cout << "Press any key to continue: ";
                        getchar();getchar();
                        cls();
                        break;

                    case 'b':
                        std :: cout << "\t\t\tEnter computer generated passkey: ";
                        std :: cin >> receivedkey;
                        decryptedText = dc.advDecryption(text, receivedkey);
                        std :: cout << decryptedText << "\n\n";

                        // body



                        std :: cout << "Press any key to continue: ";
                        getchar();getchar();
                        cls();
                        break;
                    
                    default:
                        std :: cout << "\t\t\t\t Invalid Choice!\n";
                        std :: cout << "Press any key to continue: ";
                        getchar();getchar();
                        cls();
                        break;
                }
                break;

            case 3:
                char exit_choice;
                std :: cout << "Are You Sure [Y/N]?: ";
                std :: cin >> exit_choice;

                if (exit_choice == 'Y' || exit_choice == 'y'){
                    std :: cout << "\t\t\t\t\t\t\t\t Thank You!\n\n";
                    exit(0);
                }
                else if (exit_choice == 'N' || exit_choice == 'n'){
                    
                    std :: cout << "Press any key to continue: ";
                    getchar();getchar();
                    cls();
                    break;
                }
                else
                {
                    std :: cout << "\t\t\t\t Invalid Choice!\n";
                    std :: cout << "Press any key to continue: ";
                    getchar();getchar();
                    cls();
                }
                break;
            
            default:
                getchar();
                cls();
        }
    } while (choice != 0);

}

// using namespace std; fucks things up 

// normal enc and dec for string < 20