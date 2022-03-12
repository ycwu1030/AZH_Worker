
void checking_root_file(string filename) {
    TFile *f1 = new TFile(filename.c_str());
    TTree *t1;
    f1->GetObject("Delphes", t1);
    TBranch *br = t1->GetBranch("Event");
    if (!br) {
        cout << "RES 0" << endl;
        return;
    }
    cout << "RES 1" << endl;
}
