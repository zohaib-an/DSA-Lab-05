#include <iostream>
#include <string>
using namespace std;
struct Tab {
	int t_id;                 // tab id
	string t_title, t_url;    // title and url
	Tab *nx, *prv;            // next and previous links
	Tab(int in_id, const string& in_title, const string& in_url)
		: t_id(in_id), t_title(in_title), t_url(in_url), nx(this), prv(this) {}  // node points to itself
};
class TabManager {
	Tab* cur = nullptr;       // active tab
public:
	~TabManager() {
		if (!cur) return;                      // nothing to free
		cur->prv->nx = nullptr;                // break the circle
		while (cur) { Tab* tmp = cur->nx; delete cur; cur = tmp; }  // free all nodes
	}
	void openTab(int t_id, const string& t_title, const string& t_url) {
		Tab* nw = new Tab(t_id, t_title, t_url);   // create new tab
		if (!cur) { cur = nw; return; }            // first tab
		nw->nx = cur->nx;                          // new -> old next
		nw->prv = cur;                             // new <- current
		cur->nx->prv = nw;                         // old next <- new
		cur->nx = nw;                              // current -> new
		cur = nw;                                  // new tab becomes active
	}
	void closeCurrent() {
		if (!cur) { cout << "No tabs open.\n"; return; }
		Tab* vic = cur;                            // tab to delete
		if (vic->nx == vic) cur = nullptr;         // it was the only tab
		else {
			vic->prv->nx = vic->nx;                // skip victim going forward
			vic->nx->prv = vic->prv;               // skip victim going back
			cur = vic->nx;                         // next tab becomes current
		}
		delete vic;                                // free memory
		cout << "Tab closed.\n";
	}
	void moveNext() { if (cur) cur = cur->nx; else cout << "No tabs open.\n"; }   // go forward
	void movePrev() { if (cur) cur = cur->prv; else cout << "No tabs open.\n"; }  // go back
	static void show(const Tab* tp) {          // print one tab
		cout << "ID: " << tp->t_id << " | Title: " << tp->t_title << " | URL: " << tp->t_url << "\n";
	}
	void displayCurrent() const {
		if (!cur) cout << "No tabs open.\n"; else show(cur);
	}
	void displayForward() const {
		if (!cur) { cout << "No tabs open.\n"; return; }
		Tab* tp = cur;                             // start at current
		do { show(tp); tp = tp->nx; } while (tp != cur);   // stop when back at start
	}
	void displayBackward() const {
		if (!cur) { cout << "No tabs open.\n"; return; }
		Tab* tp = cur;                             // start at current
		do { show(tp); tp = tp->prv; } while (tp != cur);  // walk backwards
	}

	void search(int t_id) const {
		if (!cur) { cout << "No tabs open.\n"; return; }
		Tab* tp = cur;
		do {
			if (tp->t_id == t_id) { show(tp); return; }    // found it
			tp = tp->nx;
		} while (tp != cur);
		cout << "Tab with ID " << t_id << " not found.\n";
	}
};
int main() {
	TabManager tm;
	int ch;                                        // menu choice
	do {
		cout << "\n--- Browser Tab Manager ---\n"
			<< "1. Open New Tab\n2. Close Current Tab\n3. Move Next\n4. Move Previous\n"
			<< "5. Display Current Tab\n6. Display All Forward\n7. Display All Backward\n"
			<< "8. Search Tab\n0. Exit\nChoice: ";
		if (!(cin >> ch)) break;                   // stop on bad input
		switch (ch) {
		case 1: {
					int t_id; string t_title, t_url;
					cout << "Tab ID: "; cin >> t_id; cin.ignore();
					cout << "Title: "; getline(cin, t_title);
					cout << "URL: "; getline(cin, t_url);
					tm.openTab(t_id, t_title, t_url);
					break;
		}
		case 2: tm.closeCurrent(); break;
		case 3: tm.moveNext(); break;
		case 4: tm.movePrev(); break;
		case 5: tm.displayCurrent(); break;
		case 6: tm.displayForward(); break;
		case 7: tm.displayBackward(); break;
		case 8: { int t_id; cout << "Tab ID: "; cin >> t_id; tm.search(t_id); break; }
		case 0: cout << "Goodbye!\n"; break;
		default: cout << "Invalid choice.\n";
		}
	} while (ch != 0);
	return 0;
}