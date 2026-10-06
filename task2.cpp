#include <iostream>
#include <string>
using namespace std;
struct Photo {
	int p_id;                          // photo id
	string p_name, p_date, p_loc;      // name, date taken, location
	Photo *nx, *prv;                   // next and previous links
	Photo(int in_id, const string& in_name, const string& in_date, const string& in_loc)
		: p_id(in_id), p_name(in_name), p_date(in_date), p_loc(in_loc), nx(this), prv(this) {}
};
class Album {
	Photo* hd = nullptr;       // first photo
	Photo* cur = nullptr;      // selected photo
	int cnt = 0;               // photo count
	Photo* find(int p_id) const {
		if (!hd) return nullptr;                   // empty album
		Photo* tmp = hd;
		do { if (tmp->p_id == p_id) return tmp; tmp = tmp->nx; } while (tmp != hd);  // one full lap
		return nullptr;                            // not found
	}
	void unlink(Photo* tmp) {
		if (tmp->nx == tmp) { hd = cur = nullptr; }    // last remaining photo
		else {
			tmp->prv->nx = tmp->nx;                // bypass forward
			tmp->nx->prv = tmp->prv;               // bypass backward
			if (hd == tmp) hd = tmp->nx;           // move head if needed
			if (cur == tmp) cur = tmp->nx;         // next becomes current
		}
		delete tmp;                                // free memory
		--cnt;
	}
	static void show(const Photo* tmp) {           // print one photo
		cout << "ID: " << tmp->p_id << " | Name: " << tmp->p_name
			<< " | Date: " << tmp->p_date << " | Location: " << tmp->p_loc << "\n";
	}
public:
	~Album() {
		if (!hd) return;
		hd->prv->nx = nullptr;                     // break the circle
		while (hd) { Photo* tmp = hd->nx; delete hd; hd = tmp; }  // free all nodes
	}
	bool addPhoto(int p_id, const string& p_name, const string& p_date, const string& p_loc) {
		if (find(p_id)) { cout << "Photo ID " << p_id << " already exists.\n"; return false; }
		Photo* nw = new Photo(p_id, p_name, p_date, p_loc);   // create node
		if (!hd) hd = cur = nw;                    // first photo
		else {
			Photo* tl = hd->prv;                   // current last photo
			nw->nx = hd; nw->prv = tl;             // link new node at both ends
			tl->nx = nw; hd->prv = nw;             // tl -> new, head <- new
		}
		++cnt;
		return true;
	}
	void insertAfterCurrent(int p_id, const string& p_name, const string& p_date, const string& p_loc) {
		if (!cur) { addPhoto(p_id, p_name, p_date, p_loc); return; }   // empty: just add
		if (find(p_id)) { cout << "Photo ID " << p_id << " already exists.\n"; return; }
		Photo* nw = new Photo(p_id, p_name, p_date, p_loc);
		nw->nx = cur->nx; nw->prv = cur;           // new sits between cur and its next
		cur->nx->prv = nw;                         // old next <- new
		cur->nx = nw;                              // cur -> new
		++cnt;
	}
	void removeById(int p_id) {
		Photo* tmp = find(p_id);                   // locate photo
		if (!tmp) { cout << "Photo not found.\n"; return; }
		unlink(tmp);
		cout << "Photo removed.\n";
	}
	void removeCurrent() {
		if (!cur) { cout << "Album is empty.\n"; return; }
		unlink(cur);
		cout << "Current photo removed.\n";
	}
	void moveNext() { if (cur) cur = cur->nx; else cout << "Album is empty.\n"; }    // forward
	void movePrev() { if (cur) cur = cur->prv; else cout << "Album is empty.\n"; }   // backward
	void displayForward() const {
		if (!cur) { cout << "Album is empty.\n"; return; }
		Photo* tmp = cur;
		do { show(tmp); tmp = tmp->nx; } while (tmp != cur);    // stop at start
	}
	void displayBackward() const {
		if (!cur) { cout << "Album is empty.\n"; return; }
		Photo* tmp = cur;
		do { show(tmp); tmp = tmp->prv; } while (tmp != cur);   // stop at start
	}
	void search(int p_id) const {
		Photo* tmp = find(p_id);
		if (tmp) show(tmp); else cout << "Photo not found.\n";
	}
	void countPhotos() const { cout << "Total photos: " << cnt << "\n"; }
};
static void readPhoto(int& p_id, string& p_name, string& p_date, string& p_loc) {
	cout << "Photo ID: "; cin >> p_id; cin.ignore();   // ignore leftover newline
	cout << "Name: "; getline(cin, p_name);
	cout << "Date Taken: "; getline(cin, p_date);
	cout << "Location: "; getline(cin, p_loc);
}
int main() {
	Album alb;
	int num;                                       // number of photos
	cout << "Number of photos: "; cin >> num;
	for (int i = 0; i < num; ++i) {
		int p_id; string p_name, p_date, p_loc;
		cout << "Photo " << i + 1 << ":\n";
		readPhoto(p_id, p_name, p_date, p_loc);
		alb.addPhoto(p_id, p_name, p_date, p_loc);
	}
	int ch;                                        // menu choice
	do {
		cout << "\n--- Circular Photo Album ---\n"
			<< "1. Add Photo (end)\n2. Insert After Current\n3. Remove by ID\n"
			<< "4. Remove Current\n5. Move Next\n6. Move Previous\n"
			<< "7. Display Forward\n8. Display Backward\n9. Search by ID\n"
			<< "10. Count Photos\n0. Exit\nChoice: ";
		if (!(cin >> ch)) break;
		int p_id; string p_name, p_date, p_loc;
		switch (ch) {
		case 1: readPhoto(p_id, p_name, p_date, p_loc); alb.addPhoto(p_id, p_name, p_date, p_loc); break;
		case 2: readPhoto(p_id, p_name, p_date, p_loc); alb.insertAfterCurrent(p_id, p_name, p_date, p_loc); break;
		case 3: cout << "Photo ID: "; cin >> p_id; alb.removeById(p_id); break;
		case 4: alb.removeCurrent(); break;
		case 5: alb.moveNext(); break;
		case 6: alb.movePrev(); break;
		case 7: alb.displayForward(); break;
		case 8: alb.displayBackward(); break;
		case 9: cout << "Photo ID: "; cin >> p_id; alb.search(p_id); break;
		case 10: alb.countPhotos(); break;
		case 0: cout << "Goodbye!\n"; break;
		default: cout << "Invalid choice.\n";
		}
	} while (ch != 0);
	return 0;
}