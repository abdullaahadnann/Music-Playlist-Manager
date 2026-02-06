#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct Song {
    string title;
    string artist;
    int duration;
    string filePath;
    Song* next;
    Song* prev;
};

Song* head = nullptr;
Song* current = nullptr;


void addSong() {
    Song* newSong = new Song;

    cin.ignore();
    cout << "Title: ";
    getline(cin, newSong->title);

    cout << "Artist: ";
    getline(cin, newSong->artist);

    cout << "Duration (seconds): ";
    cin >> newSong->duration;

    cin.ignore();
    cout << "File path (.mp3/.wav): ";
    getline(cin, newSong->filePath);

    // Convert to absolute path
    if (!fs::exists(newSong->filePath)) {
        cout << "Warning: File does not exist. Please check the path.\n";
    } else {
        newSong->filePath = fs::absolute(newSong->filePath).string();
    }

    newSong->next = nullptr;
    newSong->prev = nullptr;

    if (!head) {
        head = current = newSong;
    } else {
        Song* temp = head;
        while (temp->next)
            temp = temp->next;
        temp->next = newSong;
        newSong->prev = temp;
    }

    cout << "Song added.\n";
}

// ---------------- DISPLAY ----------------
void displayPlaylist() {
    if (!head) {
        cout << "Playlist empty.\n";
        return;
    }

    Song* temp = head;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->title << " - " << temp->artist
             << " (" << temp->duration << "s)\n";
        temp = temp->next;
    }
}

// ---------------- PLAY SONG ----------------
void playSong(Song* song) {
    if (!song) {
        cout << "No song to play.\n";
        return;
    }

    if (!fs::exists(song->filePath)) {
        cout << "Error: File not found at path: " << song->filePath << endl;
        return;
    }

    cout << "Now Playing: " << song->title << " - " << song->artist << endl;

    // Use quotes around the path to handle spaces
    string command = "start \"\" \"" + song->filePath + "\"";
    system(command.c_str());
}

// ---------------- NEXT ----------------
void playNext() {
    if (current && current->next) {
        current = current->next;
        playSong(current);
    } else {
        cout << "No next song.\n";
    }
}

// ---------------- PREVIOUS ----------------
void playPrev() {
    if (current && current->prev) {
        current = current->prev;
        playSong(current);
    } else {
        cout << "No previous song.\n";
    }
}

// ---------------- DELETE SONG ----------------
void deleteSong(string title) {
    Song* temp = head;

    while (temp && temp->title != title)
        temp = temp->next;

    if (!temp) {
        cout << "Song not found.\n";
        return;
    }

    if (temp == head)
        head = temp->next;

    if (temp->prev)
        temp->prev->next = temp->next;

    if (temp->next)
        temp->next->prev = temp->prev;

    if (current == temp)
        current = head;

    delete temp;
    cout << "Song deleted.\n";
}

// ---------------- SORT PLAYLIST ----------------
void sortPlaylist() {
    if (!head) return;

    for (Song* i = head; i->next; i = i->next) {
        for (Song* j = i->next; j; j = j->next) {
            if (i->title > j->title) {
                swap(i->title, j->title);
                swap(i->artist, j->artist);
                swap(i->duration, j->duration);
                swap(i->filePath, j->filePath);
            }
        }
    }
    cout << "Playlist sorted.\n";
}

// ---------------- SAVE FILE ----------------
void saveToFile() {
    ofstream file("playlist.txt");
    Song* temp = head;

    while (temp) {
        file << temp->title << "|"
             << temp->artist << "|"
             << temp->duration << "|"
             << temp->filePath << endl;
        temp = temp->next;
    }

    file.close();
    cout << "Playlist saved.\n";
}

void loadFromFile() {
    ifstream file("playlist.txt");
    if (!file) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find("|");
        size_t p2 = line.find("|", p1 + 1);
        size_t p3 = line.find("|", p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue;

        Song* newSong = new Song;
        newSong->title = line.substr(0, p1);
        newSong->artist = line.substr(p1 + 1, p2 - p1 - 1);

        try {
            newSong->duration = stoi(line.substr(p2 + 1, p3 - p2 - 1));
        } catch (...) {
            delete newSong;
            continue;
        }

        newSong->filePath = line.substr(p3 + 1);
        newSong->next = nullptr;
        newSong->prev = nullptr;

        if (!head) {
            head = current = newSong;
        } else {
            Song* temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newSong;
            newSong->prev = temp;
        }
    }

    file.close();
}

// ---------------- MENU ----------------
int main() {
    loadFromFile();

    int choice;
    string title;

    do {
        cout << "\n--- MUSIC PLAYLIST MANAGER ---\n";
        cout << "1. Add Song\n";
        cout << "2. Display Playlist\n";
        cout << "3. Play Current Song\n";
        cout << "4. Play Next Song\n";
        cout << "5. Play Previous Song\n";
        cout << "6. Delete Song\n";
        cout << "7. Sort Playlist\n";
        cout << "8. Save Playlist\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addSong();
            break;
        case 2:
            displayPlaylist();
            break;
        case 3:
            playSong(current);
            break;
        case 4:
            playNext();
            break;
        case 5:
            playPrev();
            break;
        case 6:
            cin.ignore();
            cout << "Enter title: ";
            getline(cin, title);
            deleteSong(title);
            break;
        case 7:
            sortPlaylist();
            break;
        case 8:
            saveToFile();
            break;
        case 0:
            saveToFile();
            cout << "Bye.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
