#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// Struct to store WR info
struct Player {
    string name;
    int offAV;
    int defAV;
    int totalAV;
    bool drafted;
};

// Class for a team
class Team {
private:
    string qbCaptain;
public:
    string wrs[8]; // 8 WRs per team
    int pickIndex = 0;

    Team(string qbName) {
        qbCaptain = qbName;
        for (int i = 0; i < 8; i++) wrs[i] = "";
    }

    void addWR(string wrName) {
        if (pickIndex < 8) {
            wrs[pickIndex++] = wrName;
        }
    }

    void displayTeam() {
        cout << "QB: " << qbCaptain << endl;
        for (int i = 0; i < 8; i++) {
            cout << "WR " << (i + 1) << ": " << wrs[i] << endl;
        }
        cout << "--------------------------\n";
    }
};

// Function Prototypes
int loadPlayers(Player players[], int maxSize);
void draftPlayers(Player players[], int playerCount, Team teams[], int teamCount);
void showAllTeams(Team teams[], int teamCount);
void saveDraftResults(Team teams[], int teamCount, string filename);

int main() {
    const int MAX_PLAYERS = 60;
    Player players[MAX_PLAYERS];
    Team teams[6] = {
        Team("Sakib"),
        Team("Ahmed"),
        Team("Nadeem"),
        Team("Fahim"),
        Team("Rayyan"),
        Team("Tariq")
    };

    int playerCount = loadPlayers(players, MAX_PLAYERS);
    if (playerCount < 48) {
        cout << "Error: Not enough players to run the draft.\n";
        return 1;
    }

    int choice;
    do {
        cout << "\n==== FLAG FOOTBALL DRAFT MENU ====\n";
        cout << "1. Run Snake Draft\n";
        cout << "2. Display Drafted Teams\n";
        cout << "3. Save Draft Results to File\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            draftPlayers(players, playerCount, teams, 6);
            cout << "Draft complete!\n";
            break;
        case 2:
            showAllTeams(teams, 6);
            break;
        case 3:
            saveDraftResults(teams, 6, "draft_results.txt");
            cout << "Results saved to draft_results.txt\n";
            break;
        case 4:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}

// Loads players from wr_list.txt
int loadPlayers(Player players[], int maxSize) {
    ifstream inFile("wr_list.txt");
    if (!inFile) {
        cout << "Error opening wr_list.txt\n";
        return 0;
    }

    int count = 0;
    while (count < maxSize && !inFile.eof()) {
        string name;
        int off, def, total;
        getline(inFile, name, ',');
        inFile >> off;
        inFile.ignore(1);
        inFile >> def;
        inFile.ignore(1);
        inFile >> total;
        inFile.ignore(1000, '\n');
        players[count] = { name, off, def, total, false };
        count++;
    }
    inFile.close();
    return count;
}

// Performs the 8-round snake draft
void draftPlayers(Player players[], int playerCount, Team teams[], int teamCount) {
    int round = 0;
    for (int r = 0; r < 8; r++) {
        if (r % 2 == 0) {
            for (int t = 0; t < teamCount; t++) {
                for (int i = 0; i < playerCount; i++) {
                    if (!players[i].drafted) {
                        teams[t].addWR(players[i].name);
                        players[i].drafted = true;
                        break;
                    }
                }
            }
        }
        else {
            for (int t = teamCount - 1; t >= 0; t--) {
                for (int i = 0; i < playerCount; i++) {
                    if (!players[i].drafted) {
                        teams[t].addWR(players[i].name);
                        players[i].drafted = true;
                        break;
                    }
                }
            }
        }
    }
}

// Displays all teams
void showAllTeams(Team teams[], int teamCount) {
    for (int i = 0; i < teamCount; i++) {
        cout << "\nTEAM " << (i + 1) << ":\n";
        teams[i].displayTeam();
    }
}

// Saves draft results to file
void saveDraftResults(Team teams[], int teamCount, string filename) {
    ofstream outFile(filename);
    for (int i = 0; i < teamCount; i++) {
        outFile << "TEAM " << (i + 1) << ":\n";
        outFile << "QB: " << teams[i].wrs[0] << "\n";
        for (int j = 0; j < 8; j++) {
            outFile << "WR " << (j + 1) << ": " << teams[i].wrs[j] << "\n";
        }
        outFile << "--------------------------\n";
    }
    outFile.close();
}
