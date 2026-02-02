#include <iostream>
#include <fstream>
using namespace std;

struct Song {
    string title;
    string artist;
    int duration;   
    Song* next;
    Song* prev;
};

class Playlist {
private:
    Song* head;     // First song in playlist
    Song* tail;     // Last song in playlist
    Song* current;  // Currently playing song

public:
    // Constructor
    Playlist() {
        head = tail = current = NULL;
    }

    // Function prototypes
    void addSong(string title, string artist, int duration);
    void displayPlaylist();
    void playCurrent();
    void playNext();
    void playPrevious();
    void searchSong(string key);
    void deleteSong(string key);
    void sortPlaylist();
    void totalDuration();
    void savePlaylist(string filename);  
    void loadPlaylist(string filename);
    void viewSavedFile(string filename);
    void appendToFile(string filename);
};

void Playlist::addSong(string title, string artist, int duration) {
    Song* newSong = new Song();
    newSong->title = title;
    newSong->artist = artist;
    newSong->duration = duration;
    newSong->next = NULL;
    newSong->prev = NULL;

    if (head == NULL) {
        head = tail = current = newSong;  // first song
    } else {
        tail->next = newSong;  // link last song to new song
        newSong->prev = tail;  // link new song back to last
        tail = newSong;         // update tail
    }

    cout << "Song added: " << title << endl;
}

void Playlist::displayPlaylist() {
    if (head == NULL) {
        cout << "Playlist is empty.\n";
        return;
    }

    Song* temp = head;  // start from the first song
    cout << "\n--- Your Playlist ---\n";
    while (temp != NULL) {
        cout << temp->title << " - " << temp->artist
             << " (" << temp->duration << " sec)" << endl;
        temp = temp->next;  // move to next song
    }
}

void Playlist::playCurrent() {
    if (current == NULL) {
        cout << "No song is playing.\n";
    } else {
        cout << "Now playing: "
             << current->title << " - "
             << current->artist << endl;
    }
}

void Playlist::playNext() {
    if (current == NULL) {
        cout << "Playlist is empty.\n";
    }
    else if (current->next == NULL) {
        cout << "You are already at the last song.\n";
    }
    else {
        current = current->next;
        playCurrent();
    }
}

void Playlist::playPrevious() {
    if (current == NULL) {
        cout << "Playlist is empty.\n";
    }
    else if (current->prev == NULL) {
        cout << "You are already at the first song.\n";
    }
    else {
        current = current->prev;
        playCurrent();
    }
}

void Playlist::searchSong(string key) {
    if (head == NULL) {
        cout << "Playlist is empty.\n";
        return;
    }

    Song* temp = head;

    while (temp != NULL) {
        if (temp->title == key) {
            cout << "Song Found!\n";
            cout << temp->title << " - "
                 << temp->artist << " ("
                 << temp->duration << " sec)\n";
            return;
        }
        temp = temp->next;
    }

    cout << "Song not found.\n";
}

void Playlist::deleteSong(string key) {
    if (head == NULL) {
        cout << "Playlist is empty.\n";
        return;
    }

    Song* temp = head;

    // search the song
    while (temp != NULL && temp->title != key) {
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "Song not found.\n";
        return;
    }

    // Case 1: Only one song
    if (head == tail) {
        head = tail = current = NULL;
    }
    // Case 2: Deleting first song
    else if (temp == head) {
        head = head->next;
        head->prev = NULL;
    }
    // Case 3: Deleting last song
    else if (temp == tail) {
        tail = tail->prev;
        tail->next = NULL;
    }
    // Case 4: Deleting middle song
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    // update current if needed
    if (current == temp) {
        current = head;
    }

    delete temp;
    cout << "Song deleted successfully.\n";
}

void Playlist::sortPlaylist() {
    if (head == NULL || head->next == NULL) {
        cout << "Not enough songs to sort.\n";
        return;
    }

    bool swapped;
    Song* temp;

    do {
        swapped = false;
        temp = head;

        while (temp->next != NULL) {
            if (temp->title > temp->next->title) {
                // swap song data
                swap(temp->title, temp->next->title);
                swap(temp->artist, temp->next->artist);
                swap(temp->duration, temp->next->duration);
                swapped = true;
            }
            temp = temp->next;
        }
    } while (swapped);

    cout << "Playlist sorted by song title.\n";
}

void Playlist::totalDuration() {
    if (head == NULL) {
        cout << "Playlist is empty.\n";
        return;
    }

    int totalSec = 0;
    Song* temp = head;
    while (temp != NULL) {
        totalSec += temp->duration;
        temp = temp->next;
    }

    int minutes = totalSec / 60;
    int seconds = totalSec % 60;

    cout << "Total Playlist Duration: " << minutes << " min " 
         << seconds << " sec\n";
}

void Playlist::savePlaylist(string filename) {
    ofstream outFile(filename); // open file for writing

    if (!outFile) {
        cout << "Error opening file for writing.\n";
        return;
    }

    Song* temp = head;
    while (temp != NULL) {
        outFile << temp->title << "|" << temp->artist << "|" << temp->duration << "\n";
        temp = temp->next;
    }

    outFile.close();
    cout << "Playlist saved to " << filename << " successfully.\n";
}

void Playlist::loadPlaylist(string filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "File not found: " << filename << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        size_t pos1 = line.find("|");
        size_t pos2 = line.rfind("|");

        string title = line.substr(0, pos1);
        string artist = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int duration = stoi(line.substr(pos2 + 1));

        addSong(title, artist, duration);
    }

    inFile.close();
    cout << "Playlist loaded from " << filename << " successfully.\n";
}

void Playlist::viewSavedFile(string filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cout << "File not found.\n";
        return;
    }

    string line;
    cout << "\n--- Saved Playlist File Content ---\n";
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    inFile.close();
}

void Playlist::appendToFile(string filename) {
    ofstream outFile(filename, ios::app); // append mode

    if (!outFile) {
        cout << "Error opening file.\n";
        return;
    }

    Song* temp = head;
    while (temp != NULL) {
        outFile << temp->title << "|" 
                << temp->artist << "|" 
                << temp->duration << "\n";
        temp = temp->next;
    }

    outFile.close();
    cout << "Songs appended to " << filename << " successfully.\n";
}

int main() {
    Playlist myPlaylist;
    int choice;
    string title, artist;
    int duration;

    do {
        cout << "\n====== MUSIC PLAYLIST MANAGER ======\n";
        cout << "1. Add Song\n";
        cout << "2. Display Playlist\n";
        cout << "3. Play Current Song\n";
        cout << "4. Play Next Song\n";
        cout << "5. Play Previous Song\n";
        cout << "6. Search Song\n";
        cout << "7. Delete Song\n";
        cout << "8. Sort Playlist (by Title)\n";
        cout << "9. Total Playlist Duration\n";
        cout << "10. Save Playlist\n";
        cout << "11. Load Playlist\n";
        cout << "12. View Saved Playlist File\n";
        cout << "13. Append Songs to Existing File\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            cin.ignore();
            cout << "Enter song title: ";
            getline(cin, title);
            cout << "Enter artist name: ";
            getline(cin, artist);
            cout << "Enter duration (in seconds): ";
            cin >> duration;
            myPlaylist.addSong(title, artist, duration);
            break;

        case 2:
            myPlaylist.displayPlaylist();
            break;

        case 3:
            myPlaylist.playCurrent();
            break;

        case 4:
            myPlaylist.playNext();
            break;

        case 5:
            myPlaylist.playPrevious();
            break;

        case 6:
            cin.ignore();
            cout << "Enter song title to search: ";
            getline(cin, title);
            myPlaylist.searchSong(title);
            break;

        case 7:
            cin.ignore();
            cout << "Enter song title to delete: ";
            getline(cin, title);
            myPlaylist.deleteSong(title);
            break;
        
        case 8:
            myPlaylist.sortPlaylist();
            break;
        
        case 9:
            myPlaylist.totalDuration();
            break;
        
        case 10:
            cin.ignore();
            cout << "Enter filename to save: ";
            getline(cin, title); // reuse title variable for filename
            myPlaylist.savePlaylist(title);
            break;

        case 11:
            cin.ignore();
            cout << "Enter filename to load: ";
            getline(cin, title); // reuse title variable for filename
            myPlaylist.loadPlaylist(title);
            break;

        case 12:
            cin.ignore();
            cout << "Enter filename to view: ";
            getline(cin, title);
            myPlaylist.viewSavedFile(title);
            break;

        case 13:
            cin.ignore();
            cout << "Enter filename to append: ";
            getline(cin, title);
            myPlaylist.appendToFile(title);
            break;

        case 0:
            cout << "Exiting Playlist Manager...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}
