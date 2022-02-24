
void remove_unused_branches(string oldfile, string newfile){
    TFile of(oldfile.c_str());
    TTree *oldtree;
    of.GetObject("Delphes",oldtree);

    for (auto deactiveBranchName : {"Event*", "Particle*", "Weight*", "Track*", "Tower*", "EFlowTrack*", "EFlowPhoton*", "EFlowNeutralHadron*", "GenJet*", "GenMissingET*","Test*"})
      oldtree->SetBranchStatus(deactiveBranchName, 0);

    TFile nf(newfile.c_str(),"recreate");
    auto newtree = oldtree->CloneTree();
    nf.Write();

}