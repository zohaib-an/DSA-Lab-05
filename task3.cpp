#include <iostream>
#include <string>
using namespace std;
struct Coach {
	int c_no;                  // coach number
	string c_type;             // coach type
	int c_cap, c_pass;         // capacity and current passengers
	Coach *nx, *prv;           // next and previous links
	Coach(int in_no, const string& in_type, int in_cap, int in_pass)
		: c_no(in_no), c_type(in_type), c_cap(in_cap), c_pass(in_pass), nx(this), prv(this) {}
	int available() const { return c_cap - c_pass; }   // empty seats
};
class Train {
	Coach* hd = nullptr;       // first coach
	Coach* cur = nullptr;      // selected coach

	Coach* find(int c_no) const {
		if (!hd) return nullptr;                   // empty train
		Coach* tmp = hd;
		do { if (tmp->c_no == c_no) return tmp; tmp = tmp->nx; } while (tmp != hd);  // one full lap
		return nullptr;                            // not found
	}
	static void show(const Coach* tmp) {           // print one coach
		cout << "Coach #" << tmp->c_no << " | Type: " << tmp->c_type
			<< " | Capacity: " << tmp->c_cap << " | Passengers: " << tmp->c_pass
			<< " | Available: " << tmp->available() << "\n";
	}
	static bool valid(int c_cap, int c_pass) {     // check seat numbers
		if (c_cap < 0 || c_pass < 0 || c_pass > c_cap) {
			cout << "Invalid data: need 0 <= passengers <= capacity.\n";
			return false;
		}
		return true;
	}
public:
	~Train() {
		if (!hd) return;
		hd->prv->nx = nullptr;                     // break the circle
		while (hd) { Coach* tmp = hd->nx; delete hd; hd = tmp; }  // free all nodes
	}
	bool addCoach(int c_no, const string& c_type, int c_cap, int c_pass) {
		if (!valid(c_cap, c_pass)) return false;
		if (find(c_no)) { cout << "Coach #" << c_no << " already exists.\n"; return false; }
		Coach* nw = new Coach(c_no, c_type, c_cap, c_pass);   // create node
		if (!hd) hd = cur = nw;                    // first coach
		else {
			Coach* tl = hd->prv;                   // current last coach
			nw->nx = hd; nw->prv = tl;             // link new node at both ends
			tl->nx = nw; hd->prv = nw;             // tl -> new, head <- new
		}
		return true;
	}
	void insertAfter(int af_no, int c_no, const string& c_type, int c_cap, int c_pass) {
		if (!valid(c_cap, c_pass)) return;
		Coach* pos = find(af_no);                  // coach to insert after
		if (!pos) { cout << "Coach #" << af_no << " not found.\n"; return; }
		if (find(c_no)) { cout << "Coach #" << c_no << " already exists.\n"; return; }
		Coach* nw = new Coach(c_no, c_type, c_cap, c_pass);
		nw->nx = pos->nx; nw->prv = pos;           // new sits between pos and its next
		pos->nx->prv = nw;                         // old next <- new
		pos->nx = nw;                              // pos -> new
	}
	void removeCoach(int c_no) {
		Coach* tmp = find(c_no);                   // locate coach
		if (!tmp) { cout << "Coach #" << c_no << " not found.\n"; return; }
		if (tmp->nx == tmp) hd = cur = nullptr;    // only coach
		else {
			tmp->prv->nx = tmp->nx;                // bypass forward
			tmp->nx->prv = tmp->prv;               // bypass backward
			if (hd == tmp) hd = tmp->nx;           // move head if needed
			if (cur == tmp) cur = tmp->nx;         // next becomes current
		}
		delete tmp;                                // free memory
		cout << "Coach removed.\n";
	}
	void moveForward()  { if (cur) cur = cur->nx;  else cout << "Train is empty.\n"; }   // forward
	void moveBackward() { if (cur) cur = cur->prv; else cout << "Train is empty.\n"; }   // backward

	void displayClockwise() const {
		if (!hd) { cout << "Train is empty.\n"; return; }
		Coach* tmp = hd;
		do { show(tmp); tmp = tmp->nx; } while (tmp != hd);     // follow nx
	}
	void displayAnticlockwise() const {
		if (!hd) { cout << "Train is empty.\n"; return; }
		Coach* tmp = hd->prv;                      // start at last coach
		do { show(tmp); tmp = tmp->prv; } while (tmp != hd->prv);   // follow prv
	}
	void search(int c_no) const {
		Coach* tmp = find(c_no);
		if (tmp) show(tmp); else cout << "Coach #" << c_no << " not found.\n";
	}
	void maxAvailable() const {
		if (!hd) { cout << "Train is empty.\n"; return; }
		int mx = hd->available();                  // best so far
		Coach* tmp = hd;
		do { if (tmp->available() > mx) mx = tmp->available(); tmp = tmp->nx; } while (tmp != hd);  // find max
		cout << "Maximum available capacity = " << mx << " seat(s):\n";
		do { if (tmp->available() == mx) show(tmp); tmp = tmp->nx; } while (tmp != hd);  // print all ties
	}
	void displayCurrent() const {
		if (cur) show(cur); else cout << "Train is empty.\n";
	}
	// Reverse the train by swapping nx/prv in every node (no data copied)
	void reverse() {
		if (!hd) { cout << "Train is empty.\n"; return; }
		Coach* tmp = hd;
		do {
			Coach* sv = tmp->nx;                   // save original next
			tmp->nx = tmp->prv;                    // swap links
			tmp->prv = sv;
			tmp = sv;                              // move on in original order
		} while (tmp != hd);
		hd = hd->nx;                               // old last coach is now first
		cout << "Train direction reversed.\n";
	}
};
static void readCoach(int& c_no, string& c_type, int& c_cap, int& c_pass) {
	cout << "Coach Number: "; cin >> c_no; cin.ignore();   // ignore leftover newline
	cout << "Coach Type: "; getline(cin, c_type);
	cout << "Passenger Capacity: "; cin >> c_cap;
	cout << "Current Passengers: "; cin >> c_pass;
}
int main() {
	Train trn;
	int num;                                       // number of coaches
	cout << "Number of coaches: "; cin >> num;
	for (int i = 0; i < num; ++i) {
		int c_no, c_cap, c_pass; string c_type;
		cout << "Coach " << i + 1 << ":\n";
		readCoach(c_no, c_type, c_cap, c_pass);
		trn.addCoach(c_no, c_type, c_cap, c_pass);
	}
	int ch;                                        // menu choice
	do {
		cout << "\n--- Train Coach Navigation System ---\n"
			<< "1. Add Coach\n2. Insert Coach After\n3. Remove Coach\n"
			<< "4. Move Forward\n5. Move Backward\n6. Display Clockwise\n"
			<< "7. Display Anti-clockwise\n8. Search Coach\n9. Max Available Capacity\n"
			<< "10. Display Current Coach\n11. Reverse Train Direction\n0. Exit\nChoice: ";
		if (!(cin >> ch)) break;
		int c_no, c_cap, c_pass, af_no; string c_type;
		switch (ch) {
		case 1: readCoach(c_no, c_type, c_cap, c_pass); trn.addCoach(c_no, c_type, c_cap, c_pass); break;
		case 2:
			cout << "Insert after coach number: "; cin >> af_no;
			readCoach(c_no, c_type, c_cap, c_pass);
			trn.insertAfter(af_no, c_no, c_type, c_cap, c_pass);
			break;
		case 3: cout << "Coach Number: "; cin >> c_no; trn.removeCoach(c_no); break;
		case 4: trn.moveForward(); break;
		case 5: trn.moveBackward(); break;
		case 6: trn.displayClockwise(); break;
		case 7: trn.displayAnticlockwise(); break;
		case 8: cout << "Coach Number: "; cin >> c_no; trn.search(c_no); break;
		case 9: trn.maxAvailable(); break;
		case 10: trn.displayCurrent(); break;
		case 11: trn.reverse(); break;
		case 0: cout << "Goodbye!\n"; break;
		default: cout << "Invalid choice.\n";
		}
	} while (ch != 0);
	return 0;
}