#include <iostream>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

void winSize(int& rows, int& columns);

namespace print{
    void header(int columns, string name = "You");
    void loginCmds(int margins);
    void controllerCmds(int margins);
    void creditorCmds(int margins);
    void collectorCmds(int margins);

    void creditorProfile(string* profileItems, int margins);
    void collectorProfile(string* profileItems, int margins);
}

namespace load{
    string* users(const string address, int& size);
    void parseUsers(string* users, string* usernames, string* passwords, bool* slots, const int &size);
    string* fetchLines(const string &address, int &size, string &header);
    void existsOrCreate(const string &address, string header);
    void decrypt(string &text);
}

namespace store{
    void login(const string address, string* usernames, string* passwords, bool* slots, const int size);
    void writeLines(const string address, string* lines, const int size, string header);
    void encrypt(string &text);
    char shiftLetter(char letter, int shift);
}

int findMatchByFirstCol(string word, string* lines, const int &size);
int findMatchByLastCol(string word, string* lines, const int &size);
bool startsWith(const string &check, const string &start);
bool endsWith(const string &check, const string &end);
bool containsChar(const string &check, char ch);
int countLines(const string &address);
string* tokenizer(const string &X, char delimiter, int& counter);
unsigned int charToInt(char num);
unsigned long long stringToDec(const string &num, const int &base = 10);
string getSubString(const string &text, int start, int num);

namespace crud{
    string viewUsers(string* usernames,  string* passwords, bool* slots, int columns, const int size);
    string viewUserProfiles(const string &address, int columns);
    void addUser(string &username, string &password, string* usernames, const int size, int margin);
    bool checkUsername(string username, string* usernames, const int size);
    bool checkPassword(string password);
    void generatePwd(string &password);
    int getEmptySlot(bool* slots, const int size);
    int searchSlot(string username, string* usernames, int size);
    void modifyUser(string &username, string &password, string* usernames, const int size, int margin);
    void deleteUser(string* usernames, string* passwords, bool* slots, int index);
    void deleteUserProfile(bool isCreditor, const string &toDelete);

    void createUserProfile(const string &address, int margins, string username);
    string getActualName(int margins);
    string getBirthDate(int margins);
    string getCurrentCity(int margins);
}

namespace request{
    void transferFiles(int margins, string username);
    string getDate(int margins);
    void create(const string &address, const string &activeUsername, int margins, string* profileItems);

    void view(const string &address, int columns, const string &activeUsername = "null");
    void viewAccepted(const string &address, int columns);
    string (*getList(const string &address, int &size, const string &activeUser = "null"))[6];
    string (*getAcceptedList(const string &address, int &size))[6];
    void editStatus(const string &address, int margins, const string &activeUser = "null");

    void acceptAssignment(const string &address, int margins, const string &activeUser, string* profileItems);
    void writeList(string list[][6], int size, const string &address);
    void writeLogList(string list[][6], int size, const string &address);

    string (*getLogList(const string &address, int &size, const string &activeUser = "null", bool isCreditor = 0))[6];
    void viewLogList(const string &address, int columns, const string &activeUser = "null", bool isCreditor = 0);
    void readLogs(const string &address, int margins, const string &activeUser = "null", bool isCreditor = 0);
    void displayLog(const string &address, int margins);
    void writeLogs(const string &address, int margins, const string &activeUser = "null", bool isCreditor = 0);
    void writeLogEntry(const string &address, int margins, string activeUser);

    void editAcceptedStatus(int margins);

    void countOngoing(const string &address, string *profileItems, bool isCreditor = 0);
}

namespace payments{
    void makePayment(string *profileItems, const int &columns, const int &margins);
    void viewPayments(int columns, const string &activeUser = "null", bool isCreditor = false);
    string (*getList(const string &address, int &size, const string &activeUser = "null", bool isCreditor = false))[8];
    int *getTotal(const string &activeUser = "null", bool isCreditor = false);
    void countAmount(string *profileItems, bool isCreditor = 0);
    void writeList(string list[][8], int size, const string &address, string toDelete, bool isCreditor);
}


int getCommandInput(int lim, int margin, int start = 0);

namespace addresses{
    const string controllerLogin = "login/controllers.csv";
    const string creditorLogin = "login/creditors.csv";
    const string collectorLogin = "login/collectors.csv";
    const string creditorProfList = "users/creditors.csv";
    const string collectorProfList = "users/collectors.csv";
    const string requests = "users/requests.csv";
    const string acceptedRequests = "users/acceptedRequests.csv";
    const string payments = "users/payments.csv";
}
namespace constant{
    const int maxYear = 2030; 
}

int main(){

    srand(time(0));

    int controllerSize = 0;                             //LOADING DATA
    string* users1 = load::users(addresses::controllerLogin, controllerSize);
    string* controllers = new string[controllerSize];
    string* controllerPwds = new string[controllerSize];
    bool* controllerSlots = new bool[controllerSize];
    load::parseUsers(users1, controllers, controllerPwds, controllerSlots, controllerSize);
    delete[] users1;
    
    int creditorSize = 0;
    string* users2 = load::users(addresses::creditorLogin, creditorSize);
    string* creditors = new string[creditorSize];
    string* creditorPwds = new string[creditorSize];
    bool* creditorSlots = new bool[creditorSize];
    load::parseUsers(users2, creditors, creditorPwds, creditorSlots, creditorSize);
    delete[] users2;

    int collectorSize = 0;
    string* users3 = load::users(addresses::collectorLogin, collectorSize);
    string* collectors = new string[collectorSize];
    string* collectorPwds = new string[collectorSize];
    bool* collectorSlots = new bool[collectorSize];
    load::parseUsers(users3, collectors, collectorPwds, collectorSlots, collectorSize);
    delete[] users3;



    bool loginScreen = 1;                               //Initial state
    bool controllerScreen = 0;
    bool creditorScreen = 0;
    bool collectorScreen = 0;
    bool on = 1;

    int rows = 0, columns = 0;

    ifstream fin;
    ofstream fout;
    stringstream messageStream;
    string message = "";

    int input = -1;

    string activeUser = " ";
    int activeUserSlot = -1;

    do {        //all encompassing loop
    
        winSize(rows, columns);
        int margins = columns / 2; 

        while (loginScreen){                                //Login Screen
            print::header(columns);
            cout << message;

            print::loginCmds(margins);
            input = getCommandInput(4, margins);

            activeUser = "";
            activeUserSlot = -1;

            string userLogin = "";
            string passwordLogin = "";
            int slotLogin = -1;

            switch (input){
                case 1:{                                    //controller
                    cout << setw(margins) << "Enter Username" << ":  ";
                    cin >> userLogin;
                    if(!cin){
                        cin.clear();
                        cin.ignore(255, '\n');
                        userLogin = "";
                    }
                    cout << setw(margins) << "Enter Password" << ":  ";
                    cin >> passwordLogin;
                    if(!cin){
                        cin.clear();
                        cin.ignore(255, '\n');
                        passwordLogin = "";
                    }
                    slotLogin = crud::searchSlot(userLogin, controllers, controllerSize);
                    if (slotLogin == -1){
                        messageStream << setw(margins) << "Invalid Username!\n\n";
                        break;
                    }
                    if (passwordLogin != controllerPwds[slotLogin]){
                        messageStream << setw(margins) << "Invalid Password!\n\n";
                        break; 
                    }
                    activeUser = userLogin;
                    activeUserSlot = slotLogin;

                    controllerScreen = 1;
                    loginScreen = 0;
                    break;
                }
                case 2:{                                    //creditor
                    cout << setw(margins) << "Enter Username" << ":  ";
                    cin >> userLogin;
                    if(!cin){
                        cin.clear();
                        cin.ignore(255, '\n');
                        userLogin = "";
                    }
                    cout << setw(margins) << "Enter Password" << ":  ";
                    cin >> passwordLogin;
                    if(!cin){
                        cin.clear();
                        cin.ignore(255, '\n');
                        passwordLogin = "";
                    }
                    slotLogin = crud::searchSlot(userLogin, creditors, creditorSize);
                    if (slotLogin == -1){
                        messageStream << setw(margins) << "Invalid Username!\n\n";
                        break;
                    }
                    if (passwordLogin != creditorPwds[slotLogin]){
                        messageStream << setw(margins) << "Invalid Password!\n\n";
                        break; 
                    }
                    activeUser = userLogin;
                    activeUserSlot = slotLogin;

                    creditorScreen = 1;
                    loginScreen = 0;
                    break;
                }
                case 3:{                                    //collector
                    cout << setw(margins) << "Enter Username" << ":  ";
                    cin >> userLogin;
                    if(!cin){
                        cin.clear();
                        cin.ignore(255, '\n');
                        userLogin = "";
                    }
                    cout << setw(margins) << "Enter Password" << ":  ";
                    cin >> passwordLogin;
                    if(!cin){
                        cin.clear();
                        cin.ignore(255, '\n');
                        passwordLogin = "";
                    }
                    slotLogin = crud::searchSlot(userLogin, collectors, collectorSize);
                    if (slotLogin == -1){
                        messageStream << setw(margins) << "Invalid Username!\n\n";
                        break;
                    }
                    if (passwordLogin != collectorPwds[slotLogin]){
                        messageStream << setw(margins) << "Invalid Password!\n\n";
                        break; 
                    }
                    activeUser = userLogin;
                    activeUserSlot = slotLogin;

                    collectorScreen = 1;
                    loginScreen = 0;
                    break;
                }
                case 4:{                                    //Create Account
                    cout << setw(margins) << ' ' << "Choose the desired user-type\n\n"
                         << setw(margins) << ' ' << "\t1. Creditor\n"
                         << setw(margins) << ' ' << "\t2. Collectors\n"
                         << setw(margins) << ' ' << ">> ";
                    int createUserType = getCommandInput(2, margins, 1);
                    int emptySlot = -1;

                    switch (createUserType){
                        case 1:     //creditor
                            emptySlot = crud::getEmptySlot(creditorSlots, creditorSize);
                            if (emptySlot == -1){
                                messageStream << "\n\t\tAll available slots already filled!\n"
                                              << "\t\tOnly 5 creditor accounts can be created per instance\n"
                                              << "\t\tRESTART application to create more accounts\n\n"; 
                                break;
                            }

                            crud::modifyUser(creditors[emptySlot], creditorPwds[emptySlot], creditors, creditorSize, margins);
                            creditorSlots[emptySlot] = 1;

                            messageStream << setw(columns / 5) << ' ' << "New Creditor " << creditors[emptySlot]
                                          << " added to the system!\n"
                                          << setw(columns / 5) << ' ' << "Password: " << creditorPwds[emptySlot]
                                          << "\n\n"; 
                            break;
                        case 2:     //Collectors
                            emptySlot = crud::getEmptySlot(collectorSlots, collectorSize);
                            if (emptySlot == -1){
                                messageStream << "\n\t\tAll available slots already filled!\n"
                                              << "\t\tOnly 5 collector accounts can be created per instance\n"
                                              << "\t\tRESTART application to create more accounts\n\n"; 
                                break;
                            }

                            crud::modifyUser(collectors[emptySlot], collectorPwds[emptySlot], collectors, collectorSize, margins);
                            collectorSlots[emptySlot] = 1;

                            messageStream << setw(columns / 5) << ' ' << "New Collector " << collectors[emptySlot]
                                          << " added to the system!\n"
                                          << setw(columns / 5) << ' ' << "Password: " << collectorPwds[emptySlot]
                                          << "\n\n";
                            break;
                        default:
                            messageStream << "\nUnexpected Input value for int createUserType\n";
                    }
                    
                    break;
                }
                case 0:
                    loginScreen = 0;
                    on = 0;
                    break;
                default:
                    messageStream << "\n\tUnexpected value for input (" << input << ") at LoginScreen!\n";     
            }
            message = messageStream.str();
            messageStream.str("");
            messageStream.clear();
        }                                                   //Login END


        while (controllerScreen){                           //controller Screen
            print::header(columns, activeUser);
            cout << message;
            print::controllerCmds(margins);
            input = getCommandInput(9, margins);

            switch (input){
                case 1:{                                        //ADD User Account
                    cout << setw(margins) << ' ' << "Choose the desired user-type to be created\n\n"
                         << setw(margins) << ' ' << "\t1. Creditor\n"
                         << setw(margins) << ' ' << "\t2. Collectors\n"
                         << setw(margins) << ' ' << ">> ";
                    int createUserType = getCommandInput(3, margins, 1);
                    int emptySlot = -1;

                    switch (createUserType){
                        case 1:     //creditor
                            emptySlot = crud::getEmptySlot(creditorSlots, creditorSize);
                            if (emptySlot == -1){
                                messageStream << "\n\t\tAll available slots already filled!\n"
                                              << "\t\tOnly 5 creditors can be added per instance\n"
                                              << "\t\tRESTART application to ADD more users\n\n"; 
                                break;
                            }

                            crud::addUser(creditors[emptySlot], creditorPwds[emptySlot], creditors, creditorSize, margins);
                            creditorSlots[emptySlot] = 1;

                            messageStream << setw(columns / 5) << ' ' << "New Creditor " << creditors[emptySlot]
                                          << " added to the system!\n"
                                          << setw(columns / 5) << ' ' << "Password: " << creditorPwds[emptySlot]
                                          << "\n\n"; 
                            break;
                        case 2:     //Collectors
                            emptySlot = crud::getEmptySlot(collectorSlots, collectorSize);
                            if (emptySlot == -1){
                                messageStream << "\n\t\tAll available slots already filled!\n"
                                              << "\t\tOnly 5 collectors can be added per instance\n"
                                              << "\t\tRESTART application to ADD more users\n\n"; 
                                break;
                            }

                            crud::addUser(collectors[emptySlot], collectorPwds[emptySlot], collectors, collectorSize, margins);
                            collectorSlots[emptySlot] = 1;

                            messageStream << setw(columns / 5) << ' ' << "New Collector " << collectors[emptySlot]
                                          << " added to the system!\n"
                                          << setw(columns / 5) << ' ' << "Password: " << collectorPwds[emptySlot]
                                          << "\n\n";
                            break;
                        case 3:     //Controllers
                            emptySlot = crud::getEmptySlot(controllerSlots, controllerSize);
                            if (emptySlot == -1){
                                messageStream << "\n\t\tAll available slots already filled!\n"
                                              << "\t\tOnly 5 controllers can be added per instance\n"
                                              << "\t\tRESTART application to ADD more users\n\n"; 
                                break;
                            }

                            crud::addUser(controllers[emptySlot], controllerPwds[emptySlot], controllers, controllerSize, margins);
                            controllerSlots[emptySlot] = 1;

                            messageStream << setw(columns / 5) << ' ' << "New Controller " << controllers[emptySlot]
                                          << " added to the system!\n"
                                          << setw(columns / 5) << ' ' << "Password: " << controllerPwds[emptySlot]
                                          << "\n\n";
                            break;
                        default:
                            messageStream << "\nUnexpected Input value for int createUserType\n";
                    }
                    
                    break;
                }
                case 2:{                                        //Modify User Account
                    cout << setw(margins) << ' ' << "Choose the desired user-type\n\n"
                         << setw(margins) << ' ' << "\t1. Creditor\n"
                         << setw(margins) << ' ' << "\t2. Collectors\n"
                         << setw(margins) << ' ' << ">> ";
                    int modifyUserType = getCommandInput(3, margins, 1);
                    
                    string toModify = "";
                    int modifySlot = -1;

                    switch (modifyUserType){
                        case 1:{
                            cout << setw(margins) << "Enter old username" << ":  ";
                            cin >> toModify;
                            if(!cin){
                                cin.clear();
                                cin.ignore(255, '\n');
                                toModify = "";
                            }
                            modifySlot = crud::searchSlot(toModify, creditors, creditorSize);
                            if (modifySlot == -1){
                                messageStream << setw(margins) << "No creditor with the username [" << toModify << "] found\n\n";
                                break;
                            }

                            crud::modifyUser(creditors[modifySlot], creditorPwds[modifySlot], creditors, creditorSize, margins);
                            messageStream << setw(columns / 5) << ' ' << "User updated Successfully!\n\n";
                            
                            break;
                        }
                        case 2:{
                            cout << setw(margins) << "Enter old username" << ":  ";
                            cin >> toModify;
                            if(!cin){
                                cin.clear();
                                cin.ignore(255, '\n');
                                toModify = "";
                            }
                            modifySlot = crud::searchSlot(toModify, collectors, collectorSize);
                            if (modifySlot == -1){
                                messageStream << setw(margins) << "No collector with the username [" << toModify << "] found\n\n";
                                break;
                            }

                            crud::modifyUser(collectors[modifySlot], collectorPwds[modifySlot], collectors, collectorSize, margins);
                            messageStream << setw(columns / 5) << ' ' << "User updated Successfully!\n\n";
                            break;
                        }
                        case 3:{
                            cout << setw(margins) << "Enter old username" << ":  ";
                            cin >> toModify;
                            if(!cin){
                                cin.clear();
                                cin.ignore(255, '\n');
                                toModify = "";
                            }
                            modifySlot = crud::searchSlot(toModify, controllers, controllerSize);
                            if (modifySlot == -1){
                                messageStream << setw(margins) << "No controller with the username [" << toModify << "] found\n\n";
                                break;
                            }

                            crud::modifyUser(controllers[modifySlot], controllerPwds[modifySlot], controllers, controllerSize, margins);
                            messageStream << setw(columns / 5) << ' ' << "User updated Successfully!\n\n";
                            break;
                        }
                        default:
                            messageStream << "\nUnexpected Input value for int modifyUserType\n";
                    }

                    break;
                }
                case 3:{                                        //Delete User Account
                    cout << setw(margins) << ' ' << "Choose the desired user-type\n\n"
                         << setw(margins) << ' ' << "\t1. Creditor\n"
                         << setw(margins) << ' ' << "\t2. Collectors\n"
                         << setw(margins) << ' ' << ">> ";
                    int deleteUserType = getCommandInput(3, margins, 1);

                    string toDelete = " ";
                    int deleteSlot = -1;
                    
                    switch (deleteUserType){
                        case 1:{
                            cout << setw(margins) << "Enter username" << ":  ";
                            cin >> toDelete;
                            if(!cin){
                                cin.clear();
                                cin.ignore(255, '\n');
                                toDelete = " ";
                            }
                            deleteSlot = crud::searchSlot(toDelete, creditors, creditorSize);
                            if (deleteSlot == -1){
                                messageStream << setw(margins) << "No creditor with the username [" << toDelete << "] found\n\n";
                                break;
                            }
                            crud::deleteUser(creditors, creditorPwds, creditorSlots, deleteSlot);
                            crud::deleteUserProfile(true, toDelete);

                            messageStream << setw(margins) << "Creditor removed successfully!" << "\n\n";

                            break;
                        }
                        case 2:{
                            cout << setw(margins) << "Enter username" << ":  ";
                            cin >> toDelete;
                            if(!cin){
                                cin.clear();
                                cin.ignore(255, '\n');
                                toDelete = " ";
                            }
                            deleteSlot = crud::searchSlot(toDelete, collectors, collectorSize);
                            if (deleteSlot == -1){
                                messageStream << setw(margins) << "No collector with the username [" << toDelete << "] found\n\n";
                                break;
                            }
                            crud::deleteUser(collectors, collectorPwds, collectorSlots, deleteSlot);
                            crud::deleteUserProfile(false, toDelete);

                            messageStream << setw(margins) << "Collector removed successfully!" << "\n\n";

                            break;
                        }
                        case 3:{
                            cout << setw(margins) << "Enter username" << ":  ";
                            cin >> toDelete;
                            if(!cin){
                                cin.clear();
                                cin.ignore(255, '\n');
                                toDelete = " ";
                            }
                            deleteSlot = crud::searchSlot(toDelete, controllers, controllerSize);
                            if (deleteSlot == -1){
                                messageStream << setw(margins) << "No controller with the username [" << toDelete << "] found\n\n";
                                break;
                            }
                            if (toDelete == activeUser){
                                messageStream << setw(margins) << "You cannot delete yourself!\n\n";
                                break;
                            }
                            crud::deleteUser(controllers, controllerPwds, controllerSlots, deleteSlot);

                            messageStream << setw(margins) << "Controller removed successfully!" << "\n\n";

                            break;
                        }
                        default:
                            messageStream << "\nUnexpected Input value for int deleteUserType\n";
                    }
                    break;
                }
                case 4:{                                        //View All Users
                    cout << setw(margins) << ' ' << "Choose the desired user-type\n\n"
                         << setw(margins) << ' ' << "\t1. Creditor\n"
                         << setw(margins) << ' ' << "\t2. Collectors\n"
                         << setw(margins) << ' ' << ">> ";
                    int viewUserType = getCommandInput(3, margins, 1);
                    switch (viewUserType){
                        case 1:{
                            cout << setw(margins) << ' ' << "Choose 1. Login Info, 2. Profiles\n"
                                 << setw(margins) << ' ' << ">> ";
                            int viewType = getCommandInput(2, margins, 1) - 1;
                            if (viewType){
                                messageStream << crud::viewUserProfiles(addresses::creditorProfList, columns);
                            } else {
                                messageStream << crud::viewUsers(creditors, creditorPwds, creditorSlots, columns, creditorSize);
                            }
                            break;
                        }
                        case 2:{
                            cout << setw(margins) << ' ' << "Choose 1. Login Info, 2. Profiles\n"
                                 << setw(margins) << ' ' << ">> ";
                            int viewType = getCommandInput(2, margins, 1) - 1;
                            if (viewType){
                                messageStream << crud::viewUserProfiles(addresses::collectorProfList, columns);
                            } else {
                                messageStream << crud::viewUsers(collectors, collectorPwds, collectorSlots, columns, collectorSize);
                            }
                            break;
                        }
                        case 3:
                            messageStream << crud::viewUsers(controllers, controllerPwds, controllerSlots, columns, controllerSize);
                            break;
                        default:
                            messageStream << "\nUnexpected Input value for int viewUserType\n";
                    }
                    break;
                }
                case 5:{                                        //Edit Accepted Requests' Status
                    print::header(columns, activeUser);
                    request::viewAccepted(addresses::requests, columns);
                    request::editAcceptedStatus(margins);
                    cout << setw(margins) << ' ' << "Enter 0 to return: ";
                    getCommandInput(0, margins);
                    break;
                }
                case 6:{                                        //View Loan Collection Requests
                    print::header(columns, activeUser);
                    request::view(addresses::requests, columns);
                    request::editStatus(addresses::requests, margins);
                    break;
                }
                case 7:{                                        //Read Logs
                    print::header(columns, activeUser);
                    request::viewLogList(addresses::acceptedRequests, columns);
                    request::readLogs(addresses::acceptedRequests, margins);
                    break;
                }
                case 8:{                                        //Write Logs
                    print::header(columns, activeUser);
                    request::viewLogList(addresses::acceptedRequests, columns);
                    request::writeLogs(addresses::acceptedRequests, margins);
                    break;
                }
                case 9:{                                        //view payments
                    print::header(columns, activeUser);
                    payments::viewPayments(columns);
                    cout << setw(margins) << ' ' << "Enter 0 to return: ";
                    getCommandInput(0, margins);
                    break;
                }
                case 0:
                    controllerScreen = 0;
                    loginScreen = 1;
                    
                    break;
                default:
                    messageStream << "\n\tUnexpected value for input (" << input << ") at controllerScreen!\n";
            }
            message = messageStream.str();
            messageStream.str("");
            messageStream.clear();

        }                                                   //controller END

        while (creditorScreen){                             //creditor Screen

            print::header(columns, activeUser);
            string csvHeader = "username,name,dob,city,totalRqsts,ongoingRqsts,totalAmountRqstd,totalAmountRcvd";
            load::existsOrCreate(addresses::creditorProfList, csvHeader);
            int profilesCount = 0;
            string* profiles = load::fetchLines(addresses::creditorProfList, profilesCount, csvHeader);

            int activeSlot = findMatchByFirstCol(activeUser, profiles, profilesCount);

            if (activeSlot == -1){
                cout << setw(margins) << "No profile found!\n\n";
                delete[] profiles;
                
                cout << setw(margins) << ' ' << "Create profile: (Enter 1 to continue) >> ";
                getCommandInput(1, margins, 1);

                crud::createUserProfile(addresses::creditorProfList, margins, activeUser);

                profiles = load::fetchLines(addresses::creditorProfList, profilesCount, csvHeader);
                activeSlot = findMatchByFirstCol(activeUser, profiles, profilesCount);
                message = "\t\t\tProfile created Successfully!\n\n";
            }

            int profileItemsSize = 0;
            string* profileItems = tokenizer(profiles[activeSlot], ',', profileItemsSize);
            request::countOngoing(addresses::acceptedRequests, profileItems, true);
            payments::countAmount(profileItems, true);

            print::header(columns, activeUser);
            cout << message;
            print::creditorCmds(margins);
            input = getCommandInput(6, margins);

            switch (input){
                case 1:{                                    //profile
                    bool profileScreen = 1;
                    while (profileScreen){
                        print::header(columns, activeUser);
                        cout << message;
                        print::creditorProfile(profileItems, margins);
                        
                        cout << setw(margins) << ' ' << "Edit profile?\n\n"
                             << setw(margins) << ' ' << "\t1. Name\n"
                             << setw(margins) << ' ' << "\t2. Birth date\n"
                             << setw(margins) << ' ' << "\t3. City of Residence\n"
                             << setw(margins) << ' ' << "\t0. Return\n\n"
                             << setw(margins) << ' ' << ">> ";
                        int profileEditInput = getCommandInput(3, margins);

                        switch (profileEditInput){
                            case 1:
                                profileItems[1] = crud::getActualName(margins);
                                messageStream << setw(margins) << "Name Updated!\n";
                                break;
                            case 2:
                                profileItems[2] = crud::getBirthDate(margins);
                                messageStream << setw(margins) << "Birth Date Updated!\n";
                                break;
                            case 3:
                                profileItems[3] = crud::getCurrentCity(margins);
                                messageStream << setw(margins) << "City Updated!\n";
                                break;
                            case 0:
                                profileScreen = 0;
                                break;
                            default:
                                messageStream << "\tSomething went wrong, report bug please\n";
                                break;
                        }
                        message = messageStream.str();
                        messageStream.str("");
                        messageStream.clear();
                    }
                    break;
                }
                case 2:{                                    //request
                    cout << setw(margins) << ' ' << "Upload valid contract\n";
                    request::transferFiles(margins, activeUser);

                    cout << setw(margins) << ' ' << "Enter essential data as per the contract:\n"
                         << setw(margins) << ' ' << "The actual name and city of residence of the Debtor will also be asked\n";
                    request::create(addresses::requests, activeUser, margins, profileItems);

                    messageStream << setw(margins) << "Request successfully made!\n";

                    break;
                }
                case 3:{                                    //negotiate
                    print::header(columns, activeUser);
                    request::view(addresses::requests, columns, activeUser);
                    request::editStatus(addresses::requests, margins, activeUser);
                    break;
                }
                case 4:{                                    //Read Logs
                    print::header(columns, activeUser);
                    request::viewLogList(addresses::acceptedRequests, columns, activeUser, true);
                    request::readLogs(addresses::acceptedRequests, margins, activeUser, true);
                    break;
                }
                case 5:{                                    //Write Logs
                    print::header(columns, activeUser);
                    request::viewLogList(addresses::acceptedRequests, columns, activeUser, true);
                    request::writeLogs(addresses::acceptedRequests, margins, activeUser, true);
                    break;
                }
                case 6:{                                    //view payments
                    print::header(columns, activeUser);
                    payments::viewPayments(columns, activeUser, true);
                    cout << setw(margins) << ' ' << "Enter 0 to return: ";
                    getCommandInput(0, margins);
                    break;
                }
                case 0:
                    
                    creditorScreen = 0;
                    loginScreen = 1;
                    
                    break;
                default:
                    messageStream << "\n\tUnexpected value for input (" << input << ") at creditorScreen!\n";
            }

            message = messageStream.str();
            messageStream.str("");
            messageStream.clear();

            for (int i = 0; i < profileItemsSize - 1; i++){
                messageStream << profileItems[i] << ',';
            }
            messageStream << profileItems[profileItemsSize - 1];
            profiles[activeSlot] = messageStream.str();

            store::writeLines(addresses::creditorProfList, profiles, profilesCount, csvHeader);
            
            delete[] profileItems;
            delete[] profiles;

            messageStream.str("");
            messageStream.clear();
        }                                                   //creditor END

        while (collectorScreen){                            //collector Screen

            print::header(columns, activeUser);
            string csvHeader = "username,name,dob,city,totalAsgnmnts,ongoingAsgnmnts,totalAmountAsngned,totalAmountCllctd";
            load::existsOrCreate(addresses::collectorProfList, csvHeader);
            int profilesCount = 0;
            string* profiles = load::fetchLines(addresses::collectorProfList, profilesCount, csvHeader);

            int activeSlot = findMatchByFirstCol(activeUser, profiles, profilesCount);

            if (activeSlot == -1){
                cout << setw(margins) << "No profile found!\n\n";
                delete[] profiles;
                
                cout << setw(margins) << ' ' << "Create profile: (Enter 1 to continue) >> ";
                getCommandInput(1, margins, 1);

                crud::createUserProfile(addresses::collectorProfList, margins, activeUser);

                profiles = load::fetchLines(addresses::collectorProfList, profilesCount, csvHeader);
                activeSlot = findMatchByFirstCol(activeUser, profiles, profilesCount);

                message = "\t\t\tProfile created Successfully!\n\n";
            }

            int profileItemsSize = 0;
            string* profileItems = tokenizer(profiles[activeSlot], ',', profileItemsSize);
            request::countOngoing(addresses::acceptedRequests, profileItems);

            print::header(columns, activeUser);
            cout << message;
            print::collectorCmds(margins);
            input = getCommandInput(6, margins);

            switch (input){
                case 1:{                                    //profile
                    bool profileScreen = 1;
                    while (profileScreen){
                        print::header(columns, activeUser);
                        cout << message;
                        print::collectorProfile(profileItems, margins);
                        
                        cout << setw(margins) << ' ' << "Edit profile?\n\n"
                             << setw(margins) << ' ' << "\t1. Name\n"
                             << setw(margins) << ' ' << "\t2. Birth date\n"
                             << setw(margins) << ' ' << "\t3. City of Residence\n"
                             << setw(margins) << ' ' << "\t0. Return\n\n"
                             << setw(margins) << ' ' << ">> ";
                        int profileEditInput = getCommandInput(3, margins);

                        switch (profileEditInput){
                            case 1:
                                profileItems[1] = crud::getActualName(margins);
                                messageStream << setw(margins) << "Name Updated!\n";
                                break;
                            case 2:
                                profileItems[2] = crud::getBirthDate(margins);
                                messageStream << setw(margins) << "Birth Date Updated!\n";
                                break;
                            case 3:
                                profileItems[3] = crud::getCurrentCity(margins);
                                messageStream << setw(margins) << "City Updated!\n";
                                break;
                            case 0:
                                profileScreen = 0;
                                break;
                            default:
                                messageStream << "\tSomething went wrong, report bug please\n";
                                break;
                        }
                        message = messageStream.str();
                        messageStream.str("");
                        messageStream.clear();
                    }
                    break;
                }
                case 2:{                                    //accept assignment
                    print::header(columns, activeUser);
                    request::viewAccepted(addresses::requests, columns);
                    request::acceptAssignment(addresses::requests, margins, activeUser, profileItems);
                    break;
                }
                case 3:{                                    //Read Logs
                    print::header(columns, activeUser);
                    request::viewLogList(addresses::acceptedRequests, columns, activeUser);
                    request::readLogs(addresses::acceptedRequests, margins, activeUser);
                    break;
                }
                case 4:{                                    //Write Logs
                    print::header(columns, activeUser);
                    request::viewLogList(addresses::acceptedRequests, columns, activeUser);
                    request::writeLogs(addresses::acceptedRequests, margins, activeUser);
                    break;
                }
                case 5:{                                    //make payments
                    print::header(columns, activeUser);
                    payments::makePayment(profileItems, columns, margins);
                    break;
                }
                case 6:{                                    //view payments
                    print::header(columns, activeUser);
                    payments::viewPayments(columns, activeUser);
                    cout << setw(margins) << ' ' << "Enter 0 to return: ";
                    getCommandInput(0, margins);
                    break;
                }
                case 0:
                    
                    collectorScreen = 0;
                    loginScreen = 1;
                    
                    break;
                default:
                    messageStream << "\n\tUnexpected value for input (" << input << ") at collectorScreen!\n";
            }

            message = messageStream.str();
            messageStream.str("");
            messageStream.clear();

            for (int i = 0; i < profileItemsSize - 1; i++){
                messageStream << profileItems[i] << ',';
            }
            messageStream << profileItems[profileItemsSize - 1];
            profiles[activeSlot] = messageStream.str();

            store::writeLines(addresses::collectorProfList, profiles, profilesCount, csvHeader);
            
            delete[] profileItems;
            delete[] profiles;

            messageStream.str("");
            messageStream.clear();
        }

    } while (on);    //END of all encompassing loop 

    store::login(addresses::controllerLogin, controllers, controllerPwds, controllerSlots, controllerSize);
    delete[] controllers;
    delete[] controllerPwds;
    delete[] controllerSlots;
    store::login(addresses::creditorLogin, creditors, creditorPwds, creditorSlots, creditorSize);
    delete[] creditors;
    delete[] creditorPwds;
    delete[] creditorSlots;
    store::login(addresses::collectorLogin, collectors, collectorPwds, collectorSlots, collectorSize);
    delete[] collectors;
    delete[] collectorPwds;
    delete[] collectorSlots;

    if (!on) 
        cout << "\n\tProgram Successfully Terminated!\n\n";
    else
        cout << "\n\tProgram Unexpectedly Terminated.\n\n";

    return 0;
}


void winSize(int& rows, int& columns){ //got this from stackoverflow, 
    CONSOLE_SCREEN_BUFFER_INFO csbi;
  
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;  
}


void print::header(int columns, string name){
    system("cls");
    
    columns /= 2;
    columns -= 29;

    cout << setfill(' ') << right << "\n\n"
         << setw(columns) << ' ' << "$$$$$$$\\  $$$$$$$$\\  $$$$$$\\   $$$$$$\\  $$\\   $$\\ $$$$$$$\\  \n"
         << setw(columns) << ' ' << "$$  __$$\\ $$  _____|$$  __$$\\ $$  __$$\\ $$ |  $$ |$$  __$$\\    \n"
         << setw(columns) << ' ' << "$$ |  $$ |$$ |      $$ /  \\__|$$ /  $$ |$$ |  $$ |$$ |  $$ |      \n"
         << setw(columns) << ' ' << "$$$$$$$  |$$$$$\\    $$ |      $$ |  $$ |$$ |  $$ |$$$$$$$  |      \n"
         << setw(columns) << ' ' << "$$  __$$< $$  __|   $$ |      $$ |  $$ |$$ |  $$ |$$  ____/        \n"
         << setw(columns) << ' ' << "$$ |  $$ |$$ |      $$ |  $$\\ $$ |  $$ |$$ |  $$ |$$ |            \n"
         << setw(columns) << ' ' << "$$ |  $$ |$$$$$$$$\\ \\$$$$$$  | $$$$$$  |\\$$$$$$  |$$ |          \n"
         << setw(columns) << ' ' << "\\__|  \\__|\\________| \\______/  \\______/  \\______/ \\__|      \n\n"
         << setw(columns) << ' ' << "Secure, Efficient, and Transparent Loan Management for [" << name << "]\n\n\n";
}

void print::loginCmds(int margins){
    cout << setw(margins) << ' ' << "Choose the desired option\n\n"
         << setw(margins) << ' ' << "\t1. Controller\n"
         << setw(margins) << ' ' << "\t2. Creditor\n"
         << setw(margins) << ' ' << "\t3. Collector\n"
         << setw(margins) << ' ' << "\t4. Create New Account\n\n"
         << setw(margins) << ' ' << "\t0. Exit\n\n"
         << setw(margins) << ' ' << ">> ";
}

void print::controllerCmds(int margins){
    cout << setw(margins) << ' ' << "Choose the desired option\n\n"
         << setw(margins) << ' ' << "\t 1| Create User Account\n" 
         << setw(margins) << ' ' << "\t 2| Modify User Account\n"
         << setw(margins) << ' ' << "\t 3| Delete User Account\n"
         << setw(margins) << ' ' << "\t 4| View All Users\n\n"
         << setw(margins) << ' ' << "\t 5| View Accepted Requests\' Status\n"
         << setw(margins) << ' ' << "\t 6| View Loan Collection Requests\n"
         << setw(margins) << ' ' << "\t 7| Read Collection Logs\n"
         << setw(margins) << ' ' << "\t 8| Write to Collection Logs\n"
         << setw(margins) << ' ' << "\t 9| View Payments\n\n" //till here rn
         << setw(margins) << ' ' << "\t 0| Log Out\n\n"
         << setw(margins) << ' ' << ">> ";
}

void print::creditorCmds(int margins){
    cout << setw(margins) << ' ' << "Choose the desired option\n\n"
         << setw(margins) << ' ' << "\t1| User Profile\n"
         << setw(margins) << ' ' << "\t2| Request Loan Collection\n"
         << setw(margins) << ' ' << "\t3| View Request Status\n"
         << setw(margins) << ' ' << "\t4| Read Logs\n"
         << setw(margins) << ' ' << "\t5| Write Logs\n"
         << setw(margins) << ' ' << "\t6| View Recieved Payments\n\n"
         << setw(margins) << ' ' << "\t0| Log Out\n\n"
         << setw(margins) << ' ' << ">> ";
}

void print::collectorCmds(int margins){
    cout << setw(margins) << ' ' << "Choose the desired option\n\n"
         << setw(margins) << ' ' << "\t1| Profile\n"
         << setw(margins) << ' ' << "\t2| Accept Collections\n"
         << setw(margins) << ' ' << "\t3| Read Logs\n"
         << setw(margins) << ' ' << "\t4| Write Logs\n"
         << setw(margins) << ' ' << "\t5| Make Payment\n"
         << setw(margins) << ' ' << "\t6| View Payments\n\n"
         << setw(margins) << ' ' << "\t0| Log Out\n\n"
         << setw(margins) << ' ' << ">> ";
}

void print::creditorProfile(string* profileItems, int margins){
    cout << setw(margins) << "Name                      : " << profileItems[1] << '\n'
         << setw(margins) << "Date of Birth             : " << profileItems[2] << '\n'
         << setw(margins) << "City of Residence         : " << profileItems[3] << '\n' 
         << setw(margins) << "Total Requests made       : " << profileItems[4] << '\n'
         << setw(margins) << "Amount of Ongoing Requests: " << profileItems[5] << '\n'
         << setw(margins) << "Total Amount Requested    : " << '$' << profileItems[6] << '\n'
         << setw(margins) << "Total Amount Recieved     : " << '$' << profileItems[7] << "\n\n";
}

void print::collectorProfile(string* profileItems, int margins){
    cout << setw(margins) << "Name                         : " << profileItems[1] << '\n'
         << setw(margins) << "Date of Birth                : " << profileItems[2] << '\n'
         << setw(margins) << "City of Residence            : " << profileItems[3] << '\n'
         << setw(margins) << "Total Assignments Accepted   : " << profileItems[4] << '\n'
         << setw(margins) << "Amount of Ongoing Assignments: " << profileItems[5] << '\n'
         << setw(margins) << "Total Amount Assigned        : " << '$' << profileItems[6] << '\n'
         << setw(margins) << "Total Amount Collected       : " << '$' << profileItems[7] << "\n\n";
}


int getCommandInput(int lim, int margin, int start){
    int inpt = 0;
    cout << setfill(' ') << right;
    cin >> inpt;
    while (!cin || (inpt < start || inpt > lim)){
        cin.clear();
        cin.ignore(255, '\n');
        cout << '\n' << setw(margin - 18) << ' ' << "Invalid Input!    >> ";
        cin >> inpt;
    }
    return inpt;
}

int countLines(const string &address){
    int count = 0;
    string line = "";
    ifstream fin(address);
    while (getline(fin, line))
        count++;
    
    fin.close();
    return count;
}

bool startsWith(const string &check, const string &start){
    int lenCheck = check.length(), lenStart = start.length();

    if (lenCheck < lenStart)
        return 0;
    
    for (int i = 0; i < lenStart; i++){
        if (check[i] != start[i])
            return 0;
    }
    return 1;
}

bool endsWith(const string &check, const string &end){
    int lenCheck = check.length(), lenEnd = end.length();

    if (lenCheck < lenEnd)
        return 0;
    
    for (int i = 1; i <= lenEnd; i++){
        if (check[lenCheck- i] != end[lenEnd - i])
            return 0;
    }
    return 1;
}

int findMatchByFirstCol(string word, string* lines, const int &size){
    word = word + ',';
    for (int i = 0; i < size; i++)
        if (startsWith(lines[i], word))
            return i;
    
    return -1;
}

int findMatchByLastCol(string word, string* lines, const int &size){
    word = ',' + word;
    for (int i = 0; i < size; i++)
        if (endsWith(lines[i], word))
            return i;
    return -1;
}

string* tokenizer(const string &X, char delimiter, int& counter){
    int sizeX = X.length();

    counter = 0;
    for (int i = 0; i < sizeX; i++){        //counting number of delimiters
        if (X[i] == delimiter)
            counter++;
    }
    counter++;
    string* arr = new string[counter];

    int delimiterIndex = -1; //to start from 0
    for (int i = 0; i < counter; i++){ //iterate over arr
        string element = "";
        for (int j = delimiterIndex + 1; j < sizeX; j++){ //iterate over string X
            if (X[j] == delimiter){
                delimiterIndex = j;
                break;
            }
            element = element + X[j];
        }
        arr[i] = element;
    }
    return arr;
}

bool containsChar(const string &check, char ch){
    for (char letter: check)
        if (letter == ch)
            return 1;
    
    return 0;
}

unsigned long long stringToDec(const string &num, const int &base){
    unsigned long long result = 0;
    unsigned long long power = 1;
    unsigned int size = num.length();

    for (int i = size - 1; i >= 0; i--){
        result += charToInt(num[i]) * power;
        if (i > 0)
            power *= base;
    }
    return result;
}

unsigned int charToInt(char num){ //0-9 normal, a-z represents 10-35
    if (num >= '0' && num <= '9'){
        return (num - '0');
    }
    if (num >= 'a' && num <= 'z'){
        return (num - 'a' + 10);
    }
    if (num >= 'A' && num <= 'Z'){
        return (num - 'A' + 10);
    }
    return 0;
}

string getSubString(const string &text, int start, int num){
    string output = "";
    int size = text.length();
    if (start + num > size) //
        num = size - start;
    
    for (int i = start; i < start + num; i++){
        output.push_back(text[i]);
    }
    return output;
}


string* load::users(const string address, int& size){
    size = 0;
    ifstream fin;
    ofstream fout;

    existsOrCreate(address, "Usernames,Passwords");
    fin.open(address);

    size = countLines(address) - 1;
    string* users = new string[size + 5];

    string line = "";
    getline(fin, line);

    for (int i = 0; i < size; i++){
        getline(fin, users[i]);
        decrypt(users[i]);
    }

    size += 5;

    for (int i = size - 5; i < size; i++)
        users[i] = "null,null";

    return users;
}

void load::parseUsers(string* users, string* usernames, string* passwords, bool* slots, const int &size){
    int delimiterIndex;
    int lineSize;
    
    for (int i = 0; i < size; i++){
        delimiterIndex = 0;
        lineSize = users[i].length();
        usernames[i] = "";
        passwords[i] = "";
        for (int j = 0; j < lineSize; j++){
            if (users[i][j] == ','){
                delimiterIndex = j;
                break;
            }
            usernames[i] += users[i][j];
        }
        for (int j = delimiterIndex + 1; j < lineSize; j++){
            passwords[i] += users[i][j];
        }
        slots[i] = (usernames[i] != "null");
    }
}

string* load::fetchLines(const string &address, int &size, string &header){
    ifstream fin(address);
    size = countLines(address) - 1;

    string* lines = new string[size];

    getline(fin, header);
    decrypt(header);

    for (int i = 0; i < size; i++){
        getline(fin, lines[i]);
        decrypt(lines[i]);
    }

    fin.close();
    return lines;
}

void load::existsOrCreate(const string &address, string header){
    ifstream fin(address);
    if(!fin){
        ofstream fout(address);
        store::encrypt(header);
        fout << header << '\n';
        fout.close();
    }
    fin.close();
}

void load::decrypt(string &text){
    string key = getSubString(text, 0, 2);
    int shift = stringToDec(key);
    shift *= -1;
    int size = text.length();

    for (int i = 2; i < size; i++){ //invert cases
        text[i] = text[i] - 32 * (text[i] >= 'a' && text[i] <= 'z') + 32 * (text[i] >= 'A' && text[i] <= 'Z');
    }
    for (int i = 2; i < size; i++){ //apply shift
        text[i] = store::shiftLetter(text[i], shift);
    }
    for (int i = 2; i < size; i++){ //invert cases again
        text[i] = text[i] - 32 * (text[i] >= 'a' && text[i] <= 'z') + 32 * (text[i] >= 'A' && text[i] <= 'Z');
    }
    text = getSubString(text, 2, size - 2);
}


void store::login(const string address, string* usernames, string* passwords, bool* slots, const int size){
    ofstream fout(address);
    string header = "Usernames,Passwords";
    encrypt(header);
    fout << header << '\n';
    string line = "";

    for (int i = 0; i < size; i++)
        if (slots[i]){
            line = usernames[i] + ',' + passwords[i];
            encrypt(line);
            fout << line << '\n';
        }

    fout.close();
}

void store::writeLines(const string address, string* lines, const int size, string header){
    ofstream fout(address);
    encrypt(header);
    fout << header << '\n';

    for (int i = 0; i < size; i++){
        encrypt(lines[i]);
        fout << lines[i] << '\n';
    }
    fout.close();
}

void store::encrypt(string &text){
    int size = text.length();

    for (int i = 0; i < size; i++){ //invert cases
        text[i] = text[i] - 32 * (text[i] >= 'a' && text[i] <= 'z') + 32 * (text[i] >= 'A' && text[i] <= 'Z');
    }
    int shift = rand() % 52 + 5;
    for (int i = 0; i < size; i++){ //apply shift
        text[i] = shiftLetter(text[i], shift);
    }
    for (int i = 0; i < size; i++){ //invert cases again
        text[i] = text[i] - 32 * (text[i] >= 'a' && text[i] <= 'z') + 32 * (text[i] >= 'A' && text[i] <= 'Z');
    }
    text = to_string(shift) + text; //add key
    if (shift < 10)
        text = '0' + text;
}

char store::shiftLetter(char letter, int shift){
    if ((letter < '0' || letter > '9') &&
        (letter < 'a' || letter > 'z') &&
        (letter < 'A' || letter > 'Z'))
        return letter;
    
    int origin = 0;
    origin = (letter - '0') * (letter >= '0' && letter <= '9');
    origin += (letter - 'a' + 10) * (letter >= 'a' && letter <= 'z');
    origin += (letter - 'A' + 36) * (letter >= 'A' && letter <= 'Z');

    shift %= 62;
    if (shift < 0)
        shift += 62; 

    origin += shift;
    origin %= 62;

    if (origin < 10)
        return '0' + origin;
    else if (origin < 36)
        return 'a' + origin - 10;
    else 
        return 'A' + origin - 36;
}


string crud::viewUserProfiles(const string &address, int columns){
    stringstream ss;
    int size = 0;
    string header = "headerNotFound";
    string *lines = load::fetchLines(address, size, header);
    int margins = columns / 6;
    int area = columns - 2 * margins;

    ss << setfill(' ') << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n"
       << setfill(' ') << setw(margins) << ' '
       << setw(area / 8) << "Username" << setw(area / 7) << " Name " << setw(area / 7) << "BirthDate"
       << setw(area / 8) << " City " << setw(area / 9) << "Total Requests" << setw(area / 9) << "Ongoing"
       << setw(area / 8) << " Total Amount " << setw(area / 8) << " Processeed" << "\n\n"
       << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    int count;
    for (int i = 0; i < size; i++){
        string* items = tokenizer(lines[i], ',', count);

        if (count < 8){
            delete[] items;
            continue;
        }
        ss << setfill(' ') << setw(margins) << i + 1
           << setw(area / 8) << items[0] << setw(area / 7) << items[1] << setw(area / 7) << items[2]
           << setw(area / 8) << items[3] << setw(area / 9) << items[4] << setw(area / 9) << items[5]
           << setw(area / 8) << items[6] << setw(area / 8) << items[7] << "\n";
    
        delete[] items;
    }
    delete[] lines;
    ss << "\n" << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    return ss.str();
}

string crud::viewUsers(string* usernames,  string* passwords, bool* slots, int columns, const int size){
    stringstream ss;
    int margins = columns / 3;
    int area = columns - 2 * margins;
    ss << setfill(' ') << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n";
    ss << setfill(' ') << setw(margins) << ' '
       << setw(area / 2) << "Usernames " << setw(area / 2) << "Passwords " << "\n\n";
    ss << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    for (int i = 0; i < size; i++){
        if (slots[i]){
            ss << setw(margins) << ' ' << setw(area / 2) << usernames[i]
               << setw(area / 2) << passwords[i] << "\n\n";
        }
    }
    ss << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    return ss.str();
}

bool crud::checkUsername(string username, string* usernames, const int size){
    int sizeU = username.length();
    if (sizeU > 16 || sizeU < 3 || username == "null")
        return 0;
        
    for (char letter: username){
        if (!((letter >= 'A' && letter <= 'Z') || 
              (letter >= 'a' && letter <= 'z') || 
              (letter >= '0' && letter <= '9'))
           )
            return 0;
    }
    for (int i = 0; i < size; i++)
        if (username == usernames[i])
            return 0;
    
    return 1;
}

bool crud::checkPassword(string password){
    int size = password.length();
    if (size > 16 || size < 3 || password == "null")
        return 0;
        
    for (char letter: password){
        if (!((letter >= 'A' && letter <= 'Z') || 
              (letter >= 'a' && letter <= 'z') || 
              (letter >= '0' && letter <= '9'))
           )
            return 0;
    }
    return 1;
}

void crud::generatePwd(string &password){
    password = "";
    int length = rand() % 4 + 6;
    bool toss = 0;
    int letter = 0;
    char elmnt = 'a';

    for (int i = 0; i < length; i++){
        toss = rand() % 2;
        letter = rand() % 26;
        elmnt = ('A' + letter) * toss + ('a' + letter) * !toss;
        password += elmnt;
    }
}

int crud::getEmptySlot(bool* slots, const int size){
    for (int i = 0; i < size; i++)
        if (!slots[i])
            return i;
    return -1;
}

int crud::searchSlot(string item, string* items, int size){
    if (item == "null")
        return -1;
    for (int i = 0; i < size; i++)
        if (item == items[i])
            return i;
    return -1;
}

void crud::addUser(string &username, string &password, string* usernames, const int size, int margin){
    string newName = "";
    cout << setfill(' ') << '\n' 
         << setw(margin - 18) << ' ' << "Enter new username: ";
    cin.ignore(255, '\n');
    getline(cin, newName);
    while (!checkUsername(newName, usernames, size)){
        cout << setw(margin) << "Invalid username! " << '\n'
             << setw(margin) << "Username must be alphanumeric, " << '\n'
             << setw(margin) << "between 3 and 16 characters, " << '\n'
             << setw(margin) << "and unique " << '\n'
             << setw(margin - 18) << ' ' << "Enter new username: ";
        getline(cin, newName);
    }
    username = newName;
    generatePwd(password);
}

void crud::modifyUser(string &username, string &password, string* usernames, const int size, int margin){
    
    addUser(username, password, usernames, size, margin);
    
    cout << setw(margin) << "New Generated password" << ": " << password << '\n'
         << setw(margin) << "Change password? (1/0) " << ">> ";
    int change = getCommandInput(1, margin);

    if (change){
        cout << setfill(' ') << '\n' 
             << setw(margin - 18) << ' ' << "Enter new password: ";
        cin.ignore(255, '\n');
        getline(cin, password);
        while (!checkPassword(password)){
            cout << setw(margin) << "Invalid password! " << '\n'
                << setw(margin) << "Password must be alphanumeric, " << '\n'
                << setw(margin) << "and between 3 and 16 characters " << '\n'
                << setw(margin - 18) << ' ' << "Enter new password: ";
            getline(cin, password);
        }
    }
}

void crud::deleteUser(string* usernames, string* passwords, bool* slots, int index){
    usernames[index] = "null";
    passwords[index] = "null";
    slots[index] = 0;
}

void crud::deleteUserProfile(bool isCreditor, const string &toDelete){
    string address = (isCreditor) ? addresses::creditorProfList : addresses::collectorProfList;
    int size = 0;
    string header = "NoHeaderFound";
    string* lines = load::fetchLines(address, size, header);

    int slot = findMatchByFirstCol(toDelete, lines, size);
    if (slot == -1){
        delete[] lines;
        return;
    }
    lines[slot] = '*' + lines[slot];

    store::writeLines(address, lines, size, header);

    delete[] lines;

    if (isCreditor){
        string (*list)[6] = request::getList(addresses::requests, size, toDelete);
        for (int i = 0; i < size; i++){
            list[i][2] = '*' + list[i][2];
        }
        request::writeList(list, size, addresses::requests);
        delete[] list;
    }

    string (*logList)[6] = request::getLogList(addresses::acceptedRequests, size, toDelete, isCreditor);
    for (int i = 0; i < size; i++)
        logList[i][isCreditor] = '*' + logList[i][isCreditor];
    
    request::writeLogList(logList, size, addresses::acceptedRequests);
    delete[] logList;

    string (*payList)[8] = payments::getList(addresses::payments, size, toDelete, isCreditor);
    for (int i = 0; i < size; i++)
        payList[i][isCreditor] = '*' + payList[i][isCreditor];
    
    payments::writeList(payList, size, addresses::payments, toDelete, isCreditor);
    delete[] payList;

}

void crud::createUserProfile(const string &address, int margins, string username){
    string elements[8];
    elements[0] = username;
    elements[1] = getActualName(margins);
    elements[2] = getBirthDate(margins);
    elements[3] = getCurrentCity(margins);
    elements[4] = "0";
    elements[5] = "0";
    elements[6] = "0";
    elements[7] = "0";

    stringstream ss;
    for (int i = 0; i < 7; i++){
        ss << elements[i] << ',';
    }
    ss << elements[7];

    string line = ss.str();
    store::encrypt(line);

    ofstream fout(address, ios::app);
    fout << line << '\n';
    fout.close();
}

string crud::getActualName(int margins){
    string name = "";
    cout << setw(margins) << ' ' << "Enter ACTUAL name: ";
    cin.ignore(255, '\n');
    getline(cin, name);
    while (containsChar(name, ',') || name.length() < 3){
        cout << setw(margins) << ' ' << "Name must not contain any commas (,): ";
        getline(cin, name);
    }
    return name;
}

string crud::getBirthDate(int margins){
    cout << setw(margins) << ' ' << "Enter your birthdate\n\n";
    return request::getDate(margins);
}

string crud::getCurrentCity(int margins){
    string city = "";
    cout << setw(margins) << ' ' << "Enter current city of residence: ";
    getline(cin, city);
    while (containsChar(city, ',') || city.length() < 2 || containsChar(city, ' ')){
        cout << setw(margins) << ' ' << "City name must not contain any commas or spaces: ";
        getline(cin, city);
    }
    return city;
}


string request::getDate(int margins){
    int year = 0, month = 0, day = 0;
    string date = "";

    cout << setw(margins) << ' ' << "Enter the year: ";
    year = getCommandInput(constant::maxYear, margins, 1900);
    date = date + to_string(year) + '-';

    cout << setw(margins) << ' ' << "Enter the month (1-12): ";
    month = getCommandInput(12, margins, 1);

    const string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    date = date + months[month - 1] + '-';

    cout << setw(margins) << ' ' << "Enter the day ";
    switch (month){
        case 2:
            if (!(year % 4) && ((year % 100) || !(year % 400))){
                cout << "(1 - 29): ";
                day = getCommandInput(29, margins, 1);
            }
            else{
                cout << "(1 - 28): ";
                day = getCommandInput(28, margins, 1);
            }
            break;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            cout << "(1 - 31): ";
            day = getCommandInput(31, margins, 1);
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            cout << "(1 - 30): ";
            day = getCommandInput(30, margins, 1);
            break;
    }
    date = date + to_string(day);
    
    return date;
}

void request::transferFiles(int margins, string username){
    cout << setw(margins) << ' ' << "Press enter to upload the file\n"
         << setw(margins) << ' ' << "The system uses magic to find your file automatically. ";
    cin.get();
    cin.clear();
    cin.ignore(255, '\n');
    const string extensions[] = {".pdf", ".docx", ".doc", ".odt"};
    cout << setw(margins) << ' ' << ". . . . .\n\n";
    cout << setw(margins) << ' ' << "File \'file_from" << username << extensions[rand() % 4] << "\' uploaded successfully!\n"; 
}

void request::create(const string &address, const string &activeUsername, int margins, string* profileItems){
    const string header = "sr,status,credUsername,totalAmountDue,quotation,DebtorName,DebtorCity,DueDate,InstallmentRate,Period";
    int amountDue = 0;
    string DebtorName = "", city = "", dueDate = "";
    int installmentRate = 100, period = 0, quotation = 100;
    
    cout << setw(margins) << ' ' << "Enter Total amount due in dollars: $";
    cin >> amountDue;
    while (!cin || amountDue < 100 || amountDue > 99'999'999){
        cin.clear();
        cin.ignore(255, '\n');
        cout << setw(margins) << "Amount must not underreach $100" << ": $";
        cin >> amountDue;
    }
    cout << setw(margins) << ' ' << "Enter the proposed commission rate in percentage\n"
         << setw(margins) << ' ' << "rate must be an integer between 0 and 100 inclusive\n"
         << setw(margins) << ' ' << ">> ";
    quotation = getCommandInput(100, margins);

    DebtorName = crud::getActualName(margins);
    city = crud::getCurrentCity(margins);
    cout << setw(margins) << ' ' << "Enter the Due date\n";
    dueDate = getDate(margins);

    cout << setw(margins) << ' ' << "Enter the installment rate in percentage\n"
         << setw(margins) << ' ' << "rate must be an integer between 0 and 100 inclusive\n"
         << setw(margins) << ' ' << ">> ";
    installmentRate = getCommandInput(100, margins);

    cout << setw(margins) << ' ' << "Enter the installment period in months\n"
         << setw(margins) << ' ' << "period must be an integer between 0 and 12 inclusive\n"
         << setw(margins) << ' ' << ">> ";
    period = getCommandInput(12, margins);

    int newTotal = stringToDec(profileItems[4]) + 1;
    // int newOngoing = stringToDec(profileItems[5]) + 1;
    int newTotalAmountRequested = stringToDec(profileItems[6]) + amountDue;

    profileItems[4] = to_string(newTotal);
    // profileItems[5] = to_string(newOngoing);
    profileItems[6] = to_string(newTotalAmountRequested);

    load::existsOrCreate(address, header);
    stringstream ss;
    ss << activeUsername << newTotal << ",pending_A," << activeUsername << ',' << amountDue << ',' << quotation << ',' << DebtorName
       << ',' << city << ',' << dueDate << ',' << installmentRate << ',' << period;
    
    string line = ss.str();
    store::encrypt(line);

    ofstream fout(address, ios::app);
    fout << line << '\n'; 
    fout.close();
}

void request::view(const string &address,  int columns, const string &activeUsername){
    const string header = "sr,status,credUsername,totalAmountDue,quotation,DebtorName,DebtorCity,DueDate,InstallmentRate,Period"; 
    load::existsOrCreate(address, header);
    ifstream fin(address);
    string line = "";
    int size = 10;
    int margins = columns / 6;
    int area = columns - 2 * margins;
    getline(fin, line);

    cout << setfill(' ') << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n"
         << setfill(' ') << setw(margins) << ' '
         << setw(area / 10) << "Status " << setw(area / 8) << "Username " << setw(area / 8) << "Total Amount"
         << setw(area / 8) << "Quotation" << setw(area / 8) << "City " << setw(area / 6) << "Due Date "
         << setw(area / 9) << " Installments " << setw(area / 11) << " Period" << "\n\n"
         << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    int count = 1;
    while(getline(fin, line)){
        load::decrypt(line);
        string* items = tokenizer(line, ',', size);

        if (size < 9){
            delete[] items;
            continue;
        }
        
        if (activeUsername == "null" || items[2] == activeUsername){
            cout << setfill(' ') << setw(margins) << count
                 << setw(area / 10) << items[1] << setw(area / 8) << items[2] << setw(area / 8) << items[3]
                 << setw(area / 8) << items[4] << setw(area / 8) << items[6] << setw(area / 6) << items[7]
                 << setw(area / 9) << items[8] << setw(area / 11) << items[9] << "\n";
            count++;
        }
        delete[] items;
    }
    fin.close();
    cout << "\n\n";
}

void request::viewAccepted(const string &address, int columns){
    const string header = "sr,status,credUsername,totalAmountDue,quotation,DebtorName,DebtorCity,DueDate,InstallmentRate,Period"; 
    load::existsOrCreate(address, header);
    ifstream fin(address);
    string line = "";
    int size = 10;
    int margins = columns / 6;
    int area = columns - 2 * margins;
    getline(fin, line);

    cout << setfill(' ') << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n"
         << setfill(' ') << setw(margins) << ' '
         << setw(area / 10) << "Status " << setw(area / 8) << "Username " << setw(area / 8) << "Total Amount"
         << setw(area / 8) << "Quotation" << setw(area / 8) << "City " << setw(area / 6) << "Due Date "
         << setw(area / 9) << " Installments " << setw(area / 11) << " Period" << "\n\n"
         << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    int count = 1;
    while(getline(fin, line)){
        load::decrypt(line);
        string* items = tokenizer(line, ',', size);
        if (size < 9){
            delete[] items;
            continue;
        }
        if (items[1] == "accepted_A" || items[1] == "assigned" || items[1] == "ongoing" || items[1] == "failed" || items[1] == "complete"){
            cout << setfill(' ') << setw(margins) << count
                 << setw(area / 10) << items[1] << setw(area / 8) << "********" << setw(area / 8) << items[3]
                 << setw(area / 8) << items[4] << setw(area / 8) << items[6] << setw(area / 6) << items[7]
                 << setw(area / 9) << items[8] << setw(area / 11) << items[9] << "\n";
            count++;
        }
        delete[] items;
    }
    fin.close();
    cout << "\n\n";
}

string (*request::getList(const string &address, int &size, const string &activeUser))[6]{
    size = countLines(address) - 1;
    string (*requests)[6] = new string[size][6];

    ifstream fin(address);
    string line = "";
    int itemCount = 0;
    getline(fin, line);
    for (int i = 0; i < size; i++){
        getline(fin, line);
        load::decrypt(line);
        string *items = tokenizer(line, ',', itemCount);
        if (itemCount < 6){
            delete[] items;
            i--;
            size--;
            continue;
        }
        for (int j = 0; j < 6; j++)    
            requests[i][j] = items[j];
        
        for (int j = 6; j < itemCount; j++){
            requests[i][5] = requests[i][5] + ',' + items[j];
        }
        
        if (activeUser != "null" && activeUser != requests[i][2]){
            i--;
            size--;
        }
        delete[] items;
    }
    fin.close();
    return requests;
}

string (*request::getAcceptedList(const string &address, int &size))[6]{
    size = countLines(address) - 1;
    string (*requests)[6] = new string[size][6];

    ifstream fin(address);
    string line = "";
    int itemCount = 0;
    getline(fin, line);
    for (int i = 0; i < size; i++){
        getline(fin, line);
        load::decrypt(line);
        string *items = tokenizer(line, ',', itemCount);
        if (itemCount < 6){
            delete[] items;
            i--;
            size--;
            continue;
        }
        for (int j = 0; j < 6; j++)    
            requests[i][j] = items[j];
        
        for (int j = 6; j < itemCount; j++){
            requests[i][5] = requests[i][5] + ',' + items[j];
        }
        if (requests[i][1] != "accepted_A" && requests[i][1] != "assigned" && requests[i][1] != "ongoing" && requests[i][1] != "failed" && requests[i][1] != "complete"){
            i--;
            size--;
        }
        delete[] items;
    }
    fin.close();
    return requests;
}

void request::editStatus(const string &address, int margins, const string &activeUser){
    int size = 0;
    string (*list)[6] = getList(address, size, activeUser);

    if (size < 1){
        delete[] list;
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
        return;
    }
    
    cout << setw(margins) << ' ' << "Choose the request by serial number: ";
    int requestNum = getCommandInput(size, margins, 1) - 1;

    cout << setw(margins) << ' ' << "Choose the desired option: \n"
         << setw(margins) << ' ' << "\t1. Accept Proposal\n"
         << setw(margins) << ' ' << "\t2. Change Quotation\n"
         << setw(margins) << ' ' << "\t3. Reject Proposal\n"
         << setw(margins) << ' ' << "\t0. Return\n\n"
         << setw(margins) << ' ' << ">> ";
    int input = getCommandInput(4, margins);

    switch (input){
        case 1:{                        //Accept
            if (activeUser == "null" && list[requestNum][1] != "pending_B"){
                list[requestNum][1] = "accepted_A";
            }
            else if (activeUser != "null" && list[requestNum][1] == "pending_B"){
                list[requestNum][1] = "accepted_B";
            }
            else {
                cout << setw(margins) << ' ' << "Cannot process!\n";
            }
            cout << setw(margins) << ' ' << "Enter 0 to return: ";
            getCommandInput(0, margins);
            break;
        }
        case 2:{                        //Change Quotation
            if (list[requestNum][1] != "assigned" && list[requestNum][1] != "accepted_A" && list[requestNum][1] != "ongoing" && list[requestNum][1] != "failed" && list[requestNum][1] != "complete"){
                if (activeUser == "null"){
                    list[requestNum][1] = "pending_B";
                    cout << setw(margins) << ' ' << "Enter the proposed commission rate in percentage\n"
                        << setw(margins) << ' ' << "rate must be an integer between 0 and 100 inclusive\n"
                        << setw(margins) << ' ' << ">> ";
                    list[requestNum][4] = to_string(getCommandInput(100, margins));
                }
                else {
                    list[requestNum][1] = "pending_A";
                    cout << setw(margins) << ' ' << "Enter the proposed commission rate in percentage\n"
                        << setw(margins) << ' ' << "rate must be an integer between 0 and 100 inclusive\n"
                        << setw(margins) << ' ' << ">> ";
                    list[requestNum][4] = to_string(getCommandInput(100, margins));
                }
            }
            cout << setw(margins) << ' ' << "Enter 0 to return: ";
            getCommandInput(0, margins);
            break;
        }
        case 3:{                        //reject
            if (activeUser == "null"){
                list[requestNum][1] = "rejected_A";
            }
            else if (list[requestNum][1] != "assigned" && list[requestNum][1] != "accepted_A" && list[requestNum][1] != "ongoing" && list[requestNum][1] != "failed" && list[requestNum][1] != "complete"){
                list[requestNum][1] = "rejected_B";
            }
            else {
                cout << setw(margins) << ' ' << "Cannot reject now!\n";
            }
            cout << setw(margins) << ' ' << "Enter 0 to return: ";
            getCommandInput(0, margins);
            break;
        }
        case 4:{
            if (activeUser == "null"){
                list[requestNum][1] = "deleted";
                list[requestNum][2].push_back('*');
            }
            cout << setw(margins) << ' ' << "Enter 0 to return: ";
            getCommandInput(0, margins);
            break;
        }
        case 0:
            break;
        default:
            break;
    }
    writeList(list, size, address);
    delete[] list;
}

void request::acceptAssignment(const string &address, int margins, const string &activeUser, string* profileItems){
    int size = 0;
    string (*list)[6] = getAcceptedList(address, size);

    if (size < 1){
        delete[] list;
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
        return;
    }
    
    cout << setw(margins) << ' ' << "Choose the request by serial number: ";
    int requestNum = getCommandInput(size, margins, 1) - 1;

    cout << setw(margins) << ' ' << "Choose the desired option: \n"
         << setw(margins) << ' ' << "\t1. Accept Assignment\n"
         << setw(margins) << ' ' << "\t0. Return\n\n"
         << setw(margins) << ' ' << ">> ";
    int input = getCommandInput(1, margins);

    if (input){                        //Accept
        if (list[requestNum][1] == "accepted_A"){
            list[requestNum][1] = "assigned";

            int newTotal = stringToDec(profileItems[4]) + 1;
            int newTotalAmountAssigned = stringToDec(profileItems[6]) + stringToDec(list[requestNum][3]);

            profileItems[4] = to_string(newTotal);
            profileItems[6] = to_string(newTotalAmountAssigned);

            string logAddress = "users/logs/" + activeUser + profileItems[4] + "_log.txt";
            
            ofstream fout;
            stringstream ss;

            fout.open(logAddress);
            if (!fout.is_open()){
                logAddress = "users/" + activeUser + profileItems[4] + "_log.txt";
                fout.open(logAddress);
            }
            ss << "Creditor: " << list[requestNum][2] << '\n';
            string text = ss.str();
            ss.str("");
            store::encrypt(text);
            fout << text;

            ss << "Collector: " << activeUser << '\n';
            text = ss.str();
            ss.str("");
            store::encrypt(text);
            fout << text;

            ss << "Request ID: " << list[requestNum][0] << '\n';
            text = ss.str();
            ss.str("");
            store::encrypt(text);
            fout << text;

            ss << "Assignment ID: " << activeUser << profileItems[4] << "\n\n";
            text = ss.str();
            ss.str("");
            store::encrypt(text);
            fout << text;
            
            fout.close();

            ss << activeUser << ',' << list[requestNum][2] << ',' << activeUser << profileItems[4] << ',' 
               << list[requestNum][0] << ",assigned," << logAddress << '\n';
            text = ss.str();
            store::encrypt(text);

            load::existsOrCreate(addresses::acceptedRequests, "coll,cred,collsr,credsr,status,logaddress");
            fout.open(addresses::acceptedRequests, ios::app);
            fout << text;
            fout.close();

        }
        else {
            cout << setw(margins) << ' ' << "Cannot process!\n";
        }
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
    }
    
    writeList(list, size, address);
    delete[] list;
}

void request::writeLogList(string list[][6], int size, const string &address){
    string header = "noHeaderFound";
    int lineCount = 0;
    string* lines = load::fetchLines(address, lineCount, header);
    
    for (int i = 0; i < size; i++){
        int activeSlot = findMatchByLastCol(list[i][5], lines, lineCount);
        if (activeSlot == -1){
            continue;
        }
        lines[activeSlot] = list[i][0] + ',' + list[i][1] + ',' + list[i][2] + ',' + list[i][3] + ',' + list[i][4] + ',' + list[i][5];
    }
    store::writeLines(address, lines, lineCount, header);

    delete[] lines;
}

void request::writeList(string list[][6], int size, const string &address){
    string header = "noHeaderFound";
    int lineCount = 0;
    string* lines = load::fetchLines(address, lineCount, header);
    
    for (int i = 0; i < size; i++){
        int activeSlot = findMatchByFirstCol(list[i][0], lines, lineCount);
        if (activeSlot == -1){
            continue;
        }
        lines[activeSlot] = list[i][0] + ',' + list[i][1] + ',' + list[i][2] + ',' + list[i][3] + ',' + list[i][4] + ',' + list[i][5];
    }
    store::writeLines(address, lines, lineCount, header);

    delete[] lines;
}

void request::viewLogList(const string &address, int columns, const string &activeUser, bool isCreditor){
    const string header = "coll,cred,collsr,credsr,status,logaddress"; 
    load::existsOrCreate(address, header);
    ifstream fin(address);
    string line = "";
    int size = 6;
    int margins = columns / 6;
    int area = columns - 2 * margins;
    getline(fin, line);

    cout << setfill(' ') << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n"
         << setfill(' ') << setw(margins) << ' '
         << setw(area / 7) << "Collector ID " << setw(area / 7) << "Creditor ID " 
         << setw(area / 6) << "AssignmentID" << setw(area / 6) << "RequestID"
         << setw(area / 7) << " Status " << setw(area / 4) << " log_Address " << "\n\n"
         << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    int count = 1;
    while(getline(fin, line)){
        load::decrypt(line);
        string* items = tokenizer(line, ',', size);
        if (size < 6){
            delete[] items;
            continue;
        }
        
        if (activeUser == items[isCreditor] || activeUser == "null"){
            cout << setfill(' ') << setw(margins) << count
                 << setw(area / 7) << items[0] << setw(area / 7) << items[1] 
                 << setw(area / 6) << items[2] << setw(area / 6) << items[3] 
                 << setw(area / 7) << items[4] << setw(area / 4) << items[5] << "\n";
            count++;
        }
        delete[] items;
    }
    fin.close();
    cout << "\n\n";
}

string (*request::getLogList(const string &address, int &size, const string &activeUser, bool isCreditor))[6]{
    size = countLines(address) - 1;
    string (*logs)[6] = new string[size][6];

    ifstream fin(address);
    string line = "";
    int itemCount = 0;
    getline(fin, line);
    for (int i = 0; i < size; i++){
        getline(fin, line);
        load::decrypt(line);
        string *items = tokenizer(line, ',', itemCount);
        if (itemCount < 6){
            delete[] items;
            i--;
            size--;
            continue;
        }
        for (int j = 0; j < 6; j++)    
            logs[i][j] = items[j];
        
        if (activeUser != "null" && activeUser != logs[i][isCreditor]){
            i--;
            size--;
        }
        delete[] items;
    }
    fin.close();
    return logs;
}

void request::readLogs(const string &address, int margins, const string &activeUser, bool isCreditor){
    int size = 0;
    string (*list)[6] = getLogList(address, size, activeUser, isCreditor);

    if (size < 1){
        delete[] list;
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
        return;
    }
    
    cout << setw(margins) << ' ' << "Choose the logs by serial number: ";
    int requestNum = getCommandInput(size, margins, 1) - 1;

    cout << setw(margins) << ' ' << "Choose the desired option: \n"
         << setw(margins) << ' ' << "\t1. Read Log\n"
         << setw(margins) << ' ' << "\t0. Return\n\n"
         << setw(margins) << ' ' << ">> ";
    int input = getCommandInput(1, margins);

    if (input){                        //read
        displayLog(list[requestNum][5], margins);
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
    }
    
    delete[] list;
}

void request::displayLog(const string &address, int margins){
    ifstream fin;
    fin.open(address);
    if (!fin){
        cout << setw(margins) << ' ' << "Could not open file!\n";
        return;
    }
    string line = "";
    margins /= 2;
    cout << setw(margins) << ' ' << ". . . . . . . . . . .\n\n";
    while (getline(fin, line)){
        load::decrypt(line);
        cout << setw(margins) << ' ' << line << '\n';
    }
    fin.close();
}

void request::writeLogEntry(const string &address, int margins, string activeUser){
    ofstream fout;
    stringstream ss;
    fout.open(address, ios::app);
    if (!fout.is_open()){
        cout << setw(margins) << ' ' << "Could not open file!\n";
        return;
    }
    
    string line = "";
    if (activeUser == "null")
        activeUser = "Administrator";

    ss << "[START]\t[" << activeUser << "]\n\n";
    line = ss.str();
    ss.str("");
    store::encrypt(line);
    fout << line;

    cin.ignore(255, '\n');
    while (true){
        cout << setw(margins) << ">> ";
        getline(cin, line);
        if (line == "END")
            break;
        store::encrypt(line);
        fout << line << '\n';
    }

    ss << "[END]\n\n";
    line = ss.str();
    store::encrypt(line);
    fout << '\n' << line;
    fout.close();
}

void request::writeLogs(const string &address, int margins, const string &activeUser, bool isCreditor){
    int size = 0;
    string (*list)[6] = getLogList(address, size, activeUser, isCreditor);

    if (size < 1){
        delete[] list;
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
        return;
    }
    
    cout << setw(margins) << ' ' << "Choose the logs by serial number: ";
    int requestNum = getCommandInput(size, margins, 1) - 1;

    cout << setw(margins) << ' ' << "Choose the desired option: \n"
         << setw(margins) << ' ' << "\t1. Add to Log\n"
         << setw(margins) << ' ' << "\t0. Return\n\n"
         << setw(margins) << ' ' << ">> ";
    int input = getCommandInput(1, margins);

    if (input){                        //read
        cout << setw(margins / 2) << ' ' << "Enter the word END in a newline to terminate write-mode\n";
        cout << setw(margins / 2) << ' ' << "Writing to " << list[requestNum][5] << '\n';
        writeLogEntry(list[requestNum][5], margins / 2, activeUser);
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
    }
    
    delete[] list;
}

void request::editAcceptedStatus(int margins){
    int size = 0;
    int logSize = 0;
    string (*list)[6] = getAcceptedList(addresses::requests, size);
    string (*logList)[6] = getLogList(addresses::acceptedRequests, logSize);

    if (size < 1){
        delete[] list;
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
        return;
    }
    
    cout << setw(margins) << ' ' << "Choose the request by serial number: ";
    int requestNum = getCommandInput(size, margins, 1) - 1;

    cout << setw(margins) << ' ' << "Choose the desired option: \n"
         << setw(margins) << ' ' << "\t1. Set as Ongoing\n"
         << setw(margins) << ' ' << "\t2. Set as Failed\n"
         << setw(margins) << ' ' << "\t3. Set as Completed\n"
         << setw(margins) << ' ' << "\t0. Return\n\n"
         << setw(margins) << ' ' << ">> ";
    int input = getCommandInput(3, margins);

    switch (input){
        case 1:{                //ongoing
            if (list[requestNum][1] == "assigned"){
                list[requestNum][1] = "ongoing";
                for (int i = 0; i < logSize; i++)
                    if (list[requestNum][0] == logList[i][3]){
                        logList[i][4] = "ongoing";
                        break;
                    }  
            }
            else {
                cout << setw(margins) << ' ' << "Cannot process!\n";
            }
            break;
        }
        case 2:{                //failed
            if (list[requestNum][1] == "ongoing"){
                list[requestNum][1] = "failed";
                for (int i = 0; i < logSize; i++)
                    if (list[requestNum][0] == logList[i][3]){
                        logList[i][4] = "failed";
                        break;
                    }  
            }
            else {
                cout << setw(margins) << ' ' << "Cannot process!\n";
            }
            break;
        }
        case 3:{                //complete
            if (list[requestNum][1] == "ongoing"){
                list[requestNum][1] = "complete";
                for (int i = 0; i < logSize; i++)
                    if (list[requestNum][0] == logList[i][3]){
                        logList[i][4] = "complete";
                        break;
                    }  
            }
            else {
                cout << setw(margins) << ' ' << "Cannot process!\n";
            }
            break;
        }
        case 0:
            break;
        default:
            break;
    }
    
    writeList(list, size, addresses::requests);
    writeLogList(logList, logSize, addresses::acceptedRequests);

    delete[] list;
    delete[] logList;
}

void request::countOngoing(const string &address, string *profileItems, bool isCreditor){
    load::existsOrCreate(address, "coll,cred,collsr,credsr,status,logaddress");
    int size = 0;
    string (*logList)[6] = getLogList(address, size, profileItems[0], isCreditor);

    int count = 0;
    for (int i = 0; i < size; i++)
        if (logList[i][4] == "ongoing")
            count++;
    
    profileItems[5] = to_string(count);
    delete[] logList;
}


void payments::makePayment(string *profileItems, const int &columns, const int &margins){
    request::viewLogList(addresses::acceptedRequests, columns, profileItems[0]);
    int logSize = 0;
    int size = 0;
    string (*logList)[6] = request::getLogList(addresses::acceptedRequests, logSize, profileItems[0]);
    string (*list)[6] = request::getAcceptedList(addresses::requests, size);

    if (logSize < 1){
        delete[] list;
        delete[] logList;
        cout << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);
        return;
    }
    
    cout << setw(margins) << ' ' << "Choose the request by serial number: ";
    int requestNum = getCommandInput(logSize, margins, 1) - 1;

    cout << setw(margins) << ' ' << "Choose the desired option: \n"
         << setw(margins) << ' ' << "\t1. Make Payment\n"
         << setw(margins) << ' ' << "\t0. Return\n\n"
         << setw(margins) << ' ' << ">> ";
    int input = getCommandInput(1, margins);

    if (input){
        int listIndex = -1;
        for (int i = 0; i < size; i++)
            if (logList[requestNum][3] == list[i][0]){
                listIndex = i;
                break;
            }
        if (listIndex == -1){
            cout << setw(margins) << ' ' << "Could not find request!\n";
            delete[] list;
            delete[] logList;
            cout << setw(margins) << ' ' << "Enter 0 to return: ";
            getCommandInput(0, margins);
            return;
        }
        
        int totalAmountDue = stringToDec(list[listIndex][3]);
        int quotation = stringToDec(list[listIndex][4]);

        cout << setw(margins) << ' ' << "Total Amount Due: " << totalAmountDue << '\n'
             << setw(margins) << ' ' << "Enter the amount to be paid (whole numbers): $";
        int amountPaid = getCommandInput(totalAmountDue, margins);

        int credAmount = amountPaid * (100 - quotation) / 100.0;
        int collAmount = amountPaid * 8 * quotation / 1000.0;
        int recoupAmount = amountPaid * 2 * quotation / 1000.0;

        string header = "coll,cred,totalAmountDue,AmountSubmitted,quotation,CollAmount,CredAmount,Recoup";
        load::existsOrCreate(addresses::payments, header);

        stringstream ss;
        ss << profileItems[0] << ',' << logList[requestNum][1] << ',' << totalAmountDue << ',' << amountPaid << ','
           << quotation << ',' << collAmount << ',' << credAmount << ',' << recoupAmount << '\n';
        string line = ss.str();
        store::encrypt(line);
        
        ofstream fout;
        fout.open(addresses::payments, ios::app);
        fout << line;
        fout.close();

        totalAmountDue -= amountPaid;
        list[listIndex][3] = to_string(totalAmountDue);
        
        amountPaid += stringToDec(profileItems[7]);
        profileItems[7] = to_string(amountPaid);

        request::writeList(list, size, addresses::requests);
        request::writeLogList(logList, size, addresses::acceptedRequests);

        delete[] list;
        delete[] logList;

        cout << setw(margins) << ' ' << "Payment successful!\n"
             << setw(margins) << ' ' << "Enter 0 to return: ";
        getCommandInput(0, margins);

    }
}

void payments::viewPayments(int columns, const string &activeUser, bool isCreditor){
    const string header = "coll,cred,totalAmountDue,AmountSubmitted,quotation,CollAmount,CredAmount,Recoup"; 
    load::existsOrCreate(addresses::payments, header);
    ifstream fin(addresses::payments);
    string line = "";
    int size = 8;
    int margins = columns / 6;
    int area = columns - 2 * margins;
    getline(fin, line);

    cout << setfill(' ') << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n"
         << setfill(' ') << setw(margins) << ' '
         << setw(area / 7) << "Collector " << setw(area / 7) << "Creditor " 
         << setw(area / 9) << " Payable " << setw(area / 9) << " Payed "
         << setw(area / 8) << "Quotation" << setw(area / 8) << "Collector\'s"
         << setw(area / 8) << "Creditor's" << setw(area / 8) << " Recoup's " << "\n\n"
         << setw(margins) << ' ' << setfill('*') << setw(area) << '*' << "\n\n" << setfill(' ');

    int count = 1;
    while(getline(fin, line)){
        load::decrypt(line);
        string* items = tokenizer(line, ',', size);
        if (size < 6){
            delete[] items;
            continue;
        }
        
        if (activeUser == items[isCreditor] || activeUser == "null"){
            cout << setfill(' ') << setw(margins) << count
                 << setw(area / 7) << items[0] << setw(area / 7) << items[1] 
                 << setw(area / 9) << items[2] << setw(area / 9) << items[3] 
                 << setw(area / 8) << items[4] << setw(area / 8) << items[5] 
                 << setw(area / 8) << items[6] << setw(area / 8) << items[7] << "\n";
            count++;
        }
        delete[] items;
    }
    fin.close();

    int *total = getTotal(activeUser, isCreditor);
    cout << setw(margins) << ' ' << setfill('_') << setw(area) << '_' << "\n\n" << setfill(' ') 
         << setw(margins + area / 2) << total[0] << setw(area / 4) << total[1] 
         << setw(area / 8) << total[2] << setw(area / 8) << total[3]
         << "\n\n";

    delete[] total;
}

string (*payments::getList(const string &address, int &size, const string &activeUser, bool isCreditor))[8]{
    size = countLines(address) - 1;
    string (*list)[8] = new string[size][8];

    ifstream fin(address);
    string line = "";
    int itemCount = 0;
    getline(fin, line);
    for (int i = 0; i < size; i++){
        getline(fin, line);
        load::decrypt(line);
        string *items = tokenizer(line, ',', itemCount);
        if (itemCount < 8){
            delete[] items;
            i--;
            size--;
            continue;
        }
        for (int j = 0; j < 8; j++)    
            list[i][j] = items[j];
        
        if (activeUser != "null" && activeUser != list[i][isCreditor]){
            i--;
            size--;
        }
        delete[] items;
    }
    fin.close();
    return list;
}

int *payments::getTotal(const string &activeUser, bool isCreditor){
    int size = 0;
    string (*list)[8] = getList(addresses::payments, size, activeUser, isCreditor);

    int collSum = 0;
    int credSum = 0;
    int payedSum = 0;
    int recoupSum = 0;

    for (int i = 0; i < size; i++){
        collSum += stringToDec(list[i][5]);
        credSum += stringToDec(list[i][6]);
        payedSum += stringToDec(list[i][3]);
        recoupSum += stringToDec(list[i][7]);
    }
    delete[] list;

    //int totals[4] = {payedSum, collSum, credSum, recoupSum};
    int *totals = new int[4];
    totals[0] = payedSum;
    totals[1] = collSum;
    totals[2] = credSum;
    totals[3] = recoupSum;
    
    return totals;
}

void payments::countAmount(string *profileItems, bool isCreditor){
    load::existsOrCreate(addresses::payments, "coll,cred,totalAmountDue,AmountSubmitted,quotation,CollAmount,CredAmount,Recoup");
    int *totals = getTotal(profileItems[0], isCreditor);
    
    profileItems[7] = to_string(totals[1 + isCreditor]);

    delete[] totals;
}

void payments::writeList(string list[][8], int size, const string &address, string toDelete, bool isCreditor){
    string header = "noHeaderFound";
    int lineCount = 0;
    string* lines = load::fetchLines(address, lineCount, header);

    string firstCol = "";
    
    for (int i = 0; i < size; i++){
        if (isCreditor){
            firstCol = list[i][0] + ',' + toDelete;
        } else {
            firstCol = toDelete + ',' + list[i][1];
        }
        int activeSlot = findMatchByFirstCol(firstCol, lines, lineCount);
        if (activeSlot == -1){
            continue;
        }
        lines[activeSlot] = list[i][0] + ',' + list[i][1] + ',' + list[i][2] + ',' + list[i][3] + ',' + list[i][4] + ',' + list[i][5] + ',' + list[i][6] + ',' + list[i][7];
    }
    store::writeLines(address, lines, lineCount, header);

    delete[] lines;
}


