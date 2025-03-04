//
//
//         Author: Jose Luis <j.l.rodriguez.sanchez@udc.es>
//
//         Last Update: 18/02/25 (Jose Luis)
//
//         Comments:
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_target_area_foot_only(const char *geoTag = "v2025_15cm", TString conf = "in")
{
    // --------------------------------------------------------------------------
    // Configurable geometry for the target vacuum chamber according to the
    // e-mail on 1st October 2019.
    // Use this macro to create root files with the different configurations
    // and positions of the vacuum chamber.
    //
    // Execute macro:  root -l
    //                 .L create_target_area_2025_15cm_geo.C
    //                 create_target_area_2025_15cm_geo()
    // --------------------------------------------------------------------------

    TGeoRotation *fRefRot = NULL;
    TGeoManager *gGeoMan = NULL;

    // -------   Load media from media file   -----------------------------------
    FairGeoLoader *geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
    FairGeoInterface *geoFace = geoLoad->getGeoInterface();
    TString geoPath = gSystem->Getenv("VMCWORKDIR");
    TString medFile = geoPath + "/geometry/media_r3b.geo";
    geoFace->setMediaFile(medFile);
    geoFace->readMedia();
    gGeoMan = gGeoManager;
    // --------------------------------------------------------------------------

    // -------   Geometry file name (output)   ----------------------------------
    TString geoFileName = geoPath + "/geometry/target_area_";
    geoFileName = geoFileName + geoTag + ".geo.root";
    // --------------------------------------------------------------------------

    // -----------------   Get and create the required media    -----------------
    FairGeoMedia *geoMedia = geoFace->getMedia();
    FairGeoBuilder *geoBuild = geoLoad->getGeoBuilder();

    FairGeoMedium *mVac = geoMedia->getMedium("vacuum");
    if (!mVac)
        Fatal("Main", "FairMedium vacuum not found");
    geoBuild->createMedium(mVac);
    TGeoMedium *pMedVac = gGeoMan->GetMedium("vacuum");
    if (!pMedVac)
        Fatal("Main", "Medium vacuum not found");

    FairGeoMedium *mCu = geoMedia->getMedium("copper");
    if (!mCu)
        Fatal("Main", "FairMedium copper not found");
    geoBuild->createMedium(mCu);
    TGeoMedium *pMed1 = gGeoMan->GetMedium("copper");
    if (!pMed1)
        Fatal("Main", "Medium copper not found");

    FairGeoMedium *mAl = geoMedia->getMedium("aluminium");
    if (!mAl)
        Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mAl);
    TGeoMedium *pMed2 = gGeoMan->GetMedium("aluminium");
    if (!pMed2)
        Fatal("Main", "Medium aluminium not found");

    FairGeoMedium *mSteel = geoMedia->getMedium("Steel");
    if (!mSteel)
        Fatal("Main", "FairMedium Steel not found");
    geoBuild->createMedium(mSteel);
    TGeoMedium *pMed3 = gGeoMan->GetMedium("Steel");
    if (!pMed3)
        Fatal("Main", "Medium Steel not found");

    FairGeoMedium *mMylar = geoMedia->getMedium("mylar");
    if (!mMylar)
        Fatal("Main", "FairMedium mylar not found");
    geoBuild->createMedium(mMylar);
    TGeoMedium *pMed4 = gGeoMan->GetMedium("mylar");
    if (!pMed4)
        Fatal("Main", "Medium mylar not found");

    FairGeoMedium *mH2 = geoMedia->getMedium("H2");
    if (!mH2)
        Fatal("Main", "FairMedium H2 not found");
    geoBuild->createMedium(mH2);
    TGeoMedium *pMed5 = gGeoMan->GetMedium("H2");
    if (!pMed5)
        Fatal("Main", "Medium H2 not found");

    FairGeoMedium *mSi = geoMedia->getMedium("silicon");
    if (!mSi)
        Fatal("Main", "FairMedium silicon not found");
    geoBuild->createMedium(mSi);
    TGeoMedium *pMedSi = gGeoMan->GetMedium("silicon");
    if (!pMedSi)
        Fatal("Main", "Medium silicon not found");

    // LOS

    FairGeoMedium *mLos = geoMedia->getMedium("BC408");
    if (!mLos)
        Fatal("Main", "FairMedium bc408 not found");
    geoBuild->createMedium(mLos);
    TGeoMedium *pMedLos = gGeoMan->GetMedium("BC408");
    if (!pMedLos)
        Fatal("Main", "Medium bc408 not found");

    // --------------   Create geometry and top volume  -------------------------
    gGeoMan = (TGeoManager *)gROOT->FindObject("FAIRGeom");
    gGeoMan->SetName("TARGETAREAgeom");
    TGeoVolume *top = new TGeoVolumeAssembly("TOP");
    gGeoMan->SetTopVolume(top);
    // --------------------------------------------------------------------------

    Double_t offsetZ =
        -0.75 * 2. +
        2.875; // offset with respect to LH2 target cell, entrance at (0,0,-0.75);
    double general_zoffset = -0.75;
    // Mylar windows for LH2 target
    Double_t thinMylar1 = 0.012 / 2.0;           // 120micra entrance window
    Double_t thinMylar2 = 0.018 / 2.0;           // 180micra exit window
    Double_t LH2targetR = 2. - thinMylar2 * 2.0; // Radius of 2 cm - mylar cylindre
    Double_t LH2targetL = 15.;                   // Length 1.5cm

    // Defintion of the Mother Volume
    auto Part1 = new TGeoTube("Part1", 0., 27., 64. / 2.0 + 30);
    auto trans1 = new TGeoCombiTrans("trans1", 0., 0., -25.5 + 0.75 - 30, fRefRot);
    trans1->RegisterYourself();

    auto Part2 = new TGeoSphere("Part2", 0., 27., 0., 90., 0., 360.);
    auto trans2 = new TGeoCombiTrans("trans2", 0, 0, 6.0 + 0.75, fRefRot);
    trans2->RegisterYourself();

    auto Part3 = new TGeoCone("Part3", 39. / 2., 0., 4.61, 0., 7.4);
    auto trans3 = new TGeoCombiTrans("trans3", 0., 0., 5.95 + 26.4 + 33.8 / 2. + 0.75, fRefRot);
    trans3->RegisterYourself();

    auto Part4 = new TGeoTube("Part4", 0., 10., 1.2 + 35);
    auto trans4 = new TGeoCombiTrans("trans4", 0., 0., 6.775000 - 0.9 + 28.812028 + 33.688 + 1. + 0.75, fRefRot);
    trans4->RegisterYourself();

    auto Part5 = new TGeoTube("Part5", 0., 29.25, 110);
    auto trans5 = new TGeoCombiTrans("trans5", 0., 0., -100, fRefRot);
    trans5->RegisterYourself();

    auto pVCWorld = new TGeoCompositeShape("VCbox", "Part1:trans1 + Part2:trans2 + Part3:trans3 + Part4:trans4+ Part5:trans5");
    TGeoVolume *pWorld = new TGeoVolume("TargetChamberWorld", pVCWorld, pMedVac);

    auto t0 = new TGeoCombiTrans(0., 0., 0., fRefRot);
    top->AddNode(pWorld, 0, t0);

    Double_t dx, dy, dz;
    Double_t thx, thy, thz;
    Double_t phx, phy, phz;

    TGeoRotation *rot0 = new TGeoRotation();

    //-------- Chamber
    //-------------------------------------------------------------------------
    /*
    Double_t parChamberBarrel[3] = {26.0, 26.4, (46.3 / 2.)};
    TGeoTube *pChamberBarrel = new TGeoTube(parChamberBarrel);
    TGeoVolume *pChamberBarrelLog =
        new TGeoVolume("ChamberBarrelLog", pChamberBarrel, pMed2);
    pChamberBarrelLog->SetVisLeaves(kTRUE);
    pChamberBarrelLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -42.1250 + 1.7 + 46.3 / 2. + offsetZ + general_zoffset;
    TGeoRotation *rot0 = new TGeoRotation();
    TGeoCombiTrans *pMatrix169 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberBarrelLog, 0, pMatrix169);

    // chamber Inner
    Double_t parChamberBarrelIn[3] = {9.625, (9.625 + 2.2), (37.57 / 4.)};
    TGeoTube *pChamberBarrelIn = new TGeoTube(parChamberBarrelIn);
    TGeoVolume *pChamberBarrelInLog =
        new TGeoVolume("ChamberBarrelInLog", pChamberBarrelIn, pMed2);
    pChamberBarrelInLog->SetVisLeaves(kTRUE);
    pChamberBarrelInLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -37.54000 - 37.57 / 4. + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix170 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberBarrelInLog, 0, pMatrix170);

    // Inner support
    Double_t parChamberIn[3] = {0., 0.6, ((37.57 - 0.9) / 2.)};
    TGeoTube *pChamberIn = new TGeoTube(parChamberIn);
    TGeoVolume *pChamberInLog = new TGeoVolume("ChamberInLog", pChamberIn, pMed2);
    pChamberInLog->SetVisLeaves(kTRUE);
    pChamberInLog->SetLineColor(34);

    dx = 9.6 - 0.6;
    dy = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix100 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 0, pMatrix100);

    dx = -(9.6 - 0.6);
    dy = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix101 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 1, pMatrix101);

    dy = -(9.6 - 0.6);
    dx = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix102 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 2, pMatrix102);

    dy = (9.6 - 0.6);
    dx = 0.000000;
    dz = -37.54000 + 0.45 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix103 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberInLog, 3, pMatrix103);

    Double_t parChamberIn2[3] = {7.6, 9.625, (0.9 / 2.)};
    TGeoTube *pChamberIn2 = new TGeoTube(parChamberIn2);
    TGeoVolume *pChamberIn2Log =
        new TGeoVolume("ChamberIn2Log", pChamberIn2, pMed2);
    pChamberIn2Log->SetVisLeaves(kTRUE);
    pChamberIn2Log->SetLineColor(17);

    dx = 0.000000;
    dy = 0.000000;
    dz = -17.8550 - 0.9 - 37.57 + 0.45 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix104 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberIn2Log, 3, pMatrix104);

    Double_t parChamberEndPlate[3] = {(9.625 + 2.2), 29.25, 1.7};
    TGeoTube *pChamberEndPlate = new TGeoTube(parChamberEndPlate);
    TGeoVolume *pChamberEndPlateLog =
        new TGeoVolume("ChamberEndPlateLog", pChamberEndPlate, pMed2);
    pChamberEndPlateLog->SetVisLeaves(kTRUE);
    pChamberEndPlateLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -42.1250 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix174 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberEndPlateLog, 0, pMatrix174);

    Double_t parChamberEndPlateIn[3] = {(3.8), (9.625 + 2.2), 0.45};
    TGeoTube *pChamberEndPlateIn = new TGeoTube(parChamberEndPlateIn);
    TGeoVolume *pChamberEndPlateInLog =
        new TGeoVolume("ChamberEndPlateInLog", pChamberEndPlateIn, pMed2);
    pChamberEndPlateInLog->SetVisLeaves(kTRUE);
    pChamberEndPlateInLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = -17.8550 - 0.45 + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix175 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pChamberEndPlateInLog, 0, pMatrix175);

    Double_t parTargetEndPlateIn[3] = {(LH2targetR + thinMylar2 * 2.0), 2.44,
                                       0.75};
    TGeoTube *pTargetEndPlateIn = new TGeoTube(parTargetEndPlateIn);
    TGeoVolume *pTargetEndPlateInLog =
        new TGeoVolume("TargetEndPlateLog", pTargetEndPlateIn, pMed2);
    pTargetEndPlateInLog->SetVisLeaves(kTRUE);
    pTargetEndPlateInLog->SetLineColor(46);

    dx = 0.000000;
    dy = 0.000000;
    dz = 0.75000 - 0.75 + general_zoffset;
    TGeoCombiTrans *pMatrix176 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pTargetEndPlateInLog, 0, pMatrix176);

    TGeoSphere *SphereChamber =
        new TGeoSphere("SphereChamber", 26.0, 26.4, 15.26, 90., 0., 360.);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9;
    TGeoCombiTrans *pMatrix177 = new TGeoCombiTrans("", dx, dy, dz, rot0);

    Double_t parTargetEndPlateOut[3] = {7.0, 10., 1.};
    TGeoTube *pTargetEndPlateOut = new TGeoTube(parTargetEndPlateOut);
    TGeoVolume *pTargetEndPlateOutLog =
        new TGeoVolume("TargetEndPlateOutLog", pTargetEndPlateOut, pMed2);
    pTargetEndPlateOutLog->SetVisLeaves(kTRUE);
    pTargetEndPlateOutLog->SetLineColor(34);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 + 1. + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix179 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pTargetEndPlateOutLog, 0, pMatrix179);

    Double_t parRingOut[5] = {(4.301 + 2.348 + 0.1), 2.048 + 0.3, 2.348 + 0.3, 0.,
                              360.};
    TGeoTorus *pRingOut = new TGeoTorus(parRingOut);

    Double_t parCone[5] = {(1.9), 4.601, 10., 4.601, 10.};
    TGeoCone *pCone = new TGeoCone(parCone);

    dx = 0.000000;
    dy = 0.000000;
    dz = 1.;
    TGeoCombiTrans *pMatrixSub = new TGeoCombiTrans("", dx, dy, dz, rot0);

    TGeoSubtraction *pSub1 = new TGeoSubtraction(pRingOut, pCone, 0, pMatrixSub);

    TGeoSphere *Sphere = new TGeoSphere("Sphere", 26.6, 30., 15.2, 25., 0., 360.);

    TGeoShape *pROutb = new TGeoCompositeShape("pROutb", pSub1);

    dx = 0.000000;
    dy = 0.000000;
    dz = -26.4 - 1.5;
    TGeoCombiTrans *pMatrixSub1 = new TGeoCombiTrans("", dx, dy, dz, rot0);

    TGeoSubtraction *pSub2 = new TGeoSubtraction(pROutb, Sphere, 0, pMatrixSub1);

    TGeoShape *pRingOutb1 = new TGeoCompositeShape("pRingOutb1", pSub2);

    Double_t parChamberCone[5] = {(33.688 / 2.), 4.301, 4.601, 7.0, 7.3};
    TGeoCone *pChamberCone = new TGeoCone(parChamberCone);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 / 2.0;
    TGeoCombiTrans *pMatrix178 = new TGeoCombiTrans("", dx, dy, dz, rot0);

    dx = 0.0000;
    dy = 0.0000;
    dz = 6.775000 - 0.9 + 28.812028 - 2.348 / 2. + 0.15;
    TGeoCombiTrans *pMatrix180 = new TGeoCombiTrans("", dx, dy, dz, rot0);

    TGeoUnion *pBoolNode0 =
        new TGeoUnion(pChamberCone, pRingOutb1, pMatrix178, pMatrix180);

    TGeoShape *pRingOutb = new TGeoCompositeShape("pNode0", pBoolNode0);

    TGeoUnion *pBoolNode1 =
        new TGeoUnion(pRingOutb, SphereChamber, 0, pMatrix177);

    TGeoShape *pFrontChamber =
        new TGeoCompositeShape("pFrontChamber", pBoolNode1);

    TGeoVolume *pFrontChamber_log =
        new TGeoVolume("FrontChamber", pFrontChamber, pMed2);
    pFrontChamber_log->SetVisLeaves(kTRUE);
    pFrontChamber_log->SetLineColor(34);

    dx = 0.0000;
    dy = 0.0000;
    dz = offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix220 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pFrontChamber_log, 0, pMatrix220);

    Double_t parCoolRingIn[5] = {2., 0.2, 0.35, 0., 90.};
    TGeoTorus *pCoolRingIn = new TGeoTorus(parCoolRingIn);
    TGeoVolume *pCoolRingIn_log =
        new TGeoVolume("CoolRingIn", pCoolRingIn, pMed1);
    pCoolRingIn_log->SetVisLeaves(kTRUE);
    pCoolRingIn_log->SetLineColor(46);

    dx = 0.000000;
    dy = 2.440000;
    dz = -2. + offsetZ + general_zoffset - 1.5;
    TGeoRotation *rot1 = new TGeoRotation();
    rot1->RotateY(-90.0);
    TGeoCombiTrans *pMatrix5 = new TGeoCombiTrans("", dx, dy, dz, rot1);
    pWorld->AddNode(pCoolRingIn_log, 0, pMatrix5);

    Double_t parCoolRingIn2[5] = {2., 0.2, 0.35, 270., 90.};
    TGeoTorus *pCoolRingIn2 = new TGeoTorus(parCoolRingIn2);
    TGeoVolume *pCoolRingIn2_log =
        new TGeoVolume("CoolRingIn2", pCoolRingIn2, pMed1);
    pCoolRingIn2_log->SetVisLeaves(kTRUE);
    pCoolRingIn2_log->SetLineColor(46);

    dx = 0.000000;
    dy = -2.44000;
    dz = -2. + offsetZ + general_zoffset - 1.5;
    TGeoCombiTrans *pMatrix6 = new TGeoCombiTrans("", dx, dy, dz, rot1);
    pWorld->AddNode(pCoolRingIn2_log, 0, pMatrix6);

    Double_t parCoolPipeIn[3] = {0.2, 0.35, ((17.855 - 1.25 - 0.75 - 1.5) / 2.)};
    TGeoTube *pCoolPipeIn = new TGeoTube(parCoolPipeIn);
    TGeoVolume *pCoolPipeIn_log =
        new TGeoVolume("CoolPipeIn", pCoolPipeIn, pMed1);
    pCoolPipeIn_log->SetVisLeaves(kTRUE);
    pCoolPipeIn_log->SetLineColor(46);

    dx = 0.000000;
    dy = -2.44000 - 2.;
    dz = -(17.855 - 1.25) / 2. - 1.25 + offsetZ - 0.75 / 2. + general_zoffset - 0.75;
    TGeoCombiTrans *pMatrix7 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pCoolPipeIn_log, 0, pMatrix7);

    dy = 2.44000 + 2.;
    TGeoCombiTrans *pMatrix8 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pCoolPipeIn_log, 1, pMatrix8);

    */

    // LH2 target --------------------------------------------------------------
    Double_t parTargetH2[3] = {0., LH2targetR, (LH2targetL / 2.0)};
    TGeoTube *pTargetH2 = new TGeoTube(parTargetH2);
    TGeoVolume *pTargetH2Log = new TGeoVolume("TargetH2Log", pTargetH2, pMed5);
    pTargetH2Log->SetVisLeaves(kTRUE);
    pTargetH2Log->SetLineColor(3);

    dx = 0.000000;
    dy = 0.000000;
    dz = LH2targetL / 2.0;
    TGeoCombiTrans *pMatrix9 =
        new TGeoCombiTrans("", dx, dy, dz + general_zoffset, rot0);
    pWorld->AddNode(pTargetH2Log, 0, pMatrix9);

    // Mylar windows for LH2 target
    Double_t parMylar1[3] = {0., LH2targetR + thinMylar2 * 2., thinMylar1};
    TGeoTube *pMylar1 = new TGeoTube(parMylar1);
    TGeoVolume *pMylar1Log =
        new TGeoVolume("MylarWindowEntranceLog", pMylar1, pMed4);
    pMylar1Log->SetVisLeaves(kTRUE);
    pMylar1Log->SetLineColor(12);

    dx = 0.000000;
    dy = 0.000000;
    dz = -thinMylar1;
    TGeoCombiTrans *pMatrix10 =
        new TGeoCombiTrans("", dx, dy, dz + general_zoffset, rot0);
    pWorld->AddNode(pMylar1Log, 0, pMatrix10);

    Double_t parMylar2[3] = {0., LH2targetR + thinMylar2 * 2., thinMylar2};
    TGeoTube *pMylar2 = new TGeoTube(parMylar2);
    TGeoVolume *pMylar2Log = new TGeoVolume("MylarWindowExitLog", pMylar2, pMed4);
    pMylar2Log->SetVisLeaves(kTRUE);
    pMylar2Log->SetLineColor(12);
    dx = 0.000000;
    dy = 0.000000;
    dz = LH2targetL + thinMylar2;
    TGeoCombiTrans *pMatrix11 =
        new TGeoCombiTrans("", dx, dy, dz + general_zoffset, rot0);
    pWorld->AddNode(pMylar2Log, 0, pMatrix11);

    Double_t parMylar3[3] = {LH2targetR, LH2targetR + thinMylar2 * 2.,
                             (LH2targetL / 2.0)};
    TGeoTube *pMylar3 = new TGeoTube(parMylar3);
    TGeoVolume *pMylar3Log = new TGeoVolume("MylarCylindreLog", pMylar3, pMed4);
    pMylar3Log->SetVisLeaves(kTRUE);
    pMylar3Log->SetLineColor(12);
    dx = 0.000000;
    dy = 0.000000;
    dz = LH2targetL / 2.0;
    TGeoCombiTrans *pMatrix12 =
        new TGeoCombiTrans("", dx, dy, dz + general_zoffset, rot0);
    pWorld->AddNode(pMylar3Log, 0, pMatrix12);

    /*

    // Exit mylar window for the vacuum chamber
    Double_t parWin[3] = {0., 7., 0.0100 / 2.0}; // 100 micras
    TGeoTube *pWin1 = new TGeoTube(parWin);
    TGeoVolume *pWin1Log = new TGeoVolume("ExitWinLog", pWin1, pMed4);
    pWin1Log->SetVisLeaves(kTRUE);
    // pWin1Log->SetLineColor(2);

    dx = 0.000000;
    dy = 0.000000;
    dz = 6.775000 - 0.9 + 28.812028 + 33.688 + 1. + offsetZ + general_zoffset;
    TGeoCombiTrans *pMatrix13 = new TGeoCombiTrans("", dx, dy, dz, rot0);
    pWorld->AddNode(pWin1Log, 0, pMatrix13);

    */

    /// LOS //////////////////////////////////////////////////////////

    /*

    // Si Shape & volume: TraBox type: TGeoBBox
    dx = 10.; // 0.5*14.50000; //5.
    dy = 10.; // 0.5*(10.00000+2.*2.62); //5.
    dz = 0.3;

    TGeoVolume *losVol = gGeoManager->MakeBox("LOS", pMedLos, dx, dy, dz);
    losVol->SetVisLeaves(kTRUE);
    losVol->SetLineColor(33);

    // Positioning
    dx = 0.;
    dy = 0.;
    dz = -94.;

    TGeoRotation *pMatrixLos1 = new TGeoRotation();
    pMatrixLos1->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrixLos2 = new TGeoCombiTrans("", dx, dy, dz, pMatrixLos1);

    pWorld->AddNode(losVol, 9, pMatrixLos2);

    */

    /// FOOT DETECTORS ///////////////////////////////////////////////////

    // FOOT Aligned on a certain Angle Trajectory
    Double_t alpha = 20.; // cover 20° trajectories

    Double_t PosZ1 = 5.;   // X position first layer
    Double_t Theta1 = 75.; // 56.6 //angle first layer
    Double_t Theta1_comp = 90. - Theta1;
    Double_t PosX1 = 5. * TMath::Sin(TMath::DegToRad() * Theta1_comp) + TMath::Tan(TMath::DegToRad() * alpha) * ((PosZ1 - 2.5) + 5 * TMath::Cos(TMath::DegToRad() * Theta1_comp)); // Z position first layer
    Double_t Gap1 = 0.82;                                                                                                                                                          // 0.5+2x0.16(PCBs)
    Double_t PosZ1b = (1. / (1. + TMath::Tan(TMath::DegToRad() * alpha) / TMath::Tan(TMath::DegToRad() * Theta1_comp))) * (PosZ1 + PosX1 / TMath::Tan(TMath::DegToRad() * Theta1_comp) - 5. * TMath::Sin(TMath::DegToRad() * Theta1_comp) / TMath::Tan(TMath::DegToRad() * Theta1_comp) + Gap1 / TMath::Sin(TMath::DegToRad() * Theta1_comp) - 5. * TMath::Cos(TMath::DegToRad() * Theta1_comp) * TMath::Tan(TMath::DegToRad() * alpha) / TMath::Tan(TMath::DegToRad() * Theta1_comp) + 2.5 * TMath::Tan(TMath::DegToRad() * alpha) / TMath::Tan(TMath::DegToRad() * Theta1_comp));
    Double_t PosX1b = 5. * TMath::Sin(TMath::DegToRad() * Theta1_comp) + TMath::Tan(TMath::DegToRad() * alpha) * ((PosZ1b - 2.5) + 5 * TMath::Cos(TMath::DegToRad() * Theta1_comp));

    Double_t PosZ2 = 9.;   // X position second layer
    Double_t Theta2 = 75.; // 56.6 //angle second layer
    Double_t Theta2_comp = 90. - Theta2;
    Double_t PosX2 = 5. * TMath::Sin(TMath::DegToRad() * Theta2_comp) + TMath::Tan(TMath::DegToRad() * alpha) * ((PosZ2 - 2.5) + 5 * TMath::Cos(TMath::DegToRad() * Theta2_comp)); // Z position second layer
    Double_t Gap2 = 0.82;                                                                                                                                                          // 0.5+2x0.16(PCBs)
    Double_t PosZ2b = (1. / (1. + TMath::Tan(TMath::DegToRad() * alpha) / TMath::Tan(TMath::DegToRad() * Theta2_comp))) * (PosZ2 + PosX2 / TMath::Tan(TMath::DegToRad() * Theta2_comp) - 5. * TMath::Sin(TMath::DegToRad() * Theta2_comp) / TMath::Tan(TMath::DegToRad() * Theta2_comp) + Gap2 / TMath::Sin(TMath::DegToRad() * Theta2_comp) - 5. * TMath::Cos(TMath::DegToRad() * Theta2_comp) * TMath::Tan(TMath::DegToRad() * alpha) / TMath::Tan(TMath::DegToRad() * Theta2_comp) + 2.5 * TMath::Tan(TMath::DegToRad() * alpha) / TMath::Tan(TMath::DegToRad() * Theta2_comp));
    Double_t PosX2b = 5. * TMath::Sin(TMath::DegToRad() * Theta2_comp) + TMath::Tan(TMath::DegToRad() * alpha) * ((PosZ2b - 2.5) + 5 * TMath::Cos(TMath::DegToRad() * Theta2_comp));

    Double_t PosZ3 = 20.; // Z position beam tracker
    Double_t Gap3 = 0.5;

    // Si Shape & volume: TraBox type: TGeoBBox
    dx = 5.; // 0.5*14.50000; //5.
    dy = 5.; // 0.5*(10.00000+2.*2.62); //5.
    dz = 0.0075000;

    // Volume: FOOT_
    TGeoVolume *TraLog = gGeoManager->MakeBox("Foot_", pMedSi, dx, dy, dz);
    TraLog->SetVisLeaves(kTRUE);
    TraLog->SetLineColor(33);

    Double_t z1, z2, z3, z4, z5, z6, z7, z8;

    /*

    z1 = -89.;
    z2 = -85.;
    z3 = -65.;
    z4 = -61.;

    if (conf == "in")
    {
      z5 = 26.;
      z6 = 30.;

      z7 = 76.;
      z8 = 80.;
    }

    if (conf == "out")
    {
      z5 = 76.;
      z6 = 80.;

      z7 = 100.;
      z8 = 104.;
    }



    z1 = -87.;
    z2 = -85.;
    z3 = -63.;
    z4 = -61.;

    if (conf == "in")
    {
      z5 = 26.;
      z6 = 28.;

      z7 = 76.;
      z8 = 78.;
    }

    if (conf == "out")
    {
      z5 = 76.;
      z6 = 80.;

      z7 = 100.;
      z8 = 104.;
    }

    */

    z1 = -200;
    z2 = -198;
    z3 = -102;
    z4 = -98;

    if (conf == "in")
    {
        z5 = 27.;
        z6 = 29.;

        z7 = 72.;
        z8 = 74.;
    }

    if (conf == "out")
    {
        z5 = 76.;
        z6 = 80.;

        z7 = 100.;
        z8 = 104.;
    }

    // Nº 2
    dx = 0.;
    dy = 0.;
    dz = z2;
    TGeoRotation *pMatrix203 = new TGeoRotation();
    pMatrix203->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix202 = new TGeoCombiTrans("", dx, dy, dz, pMatrix203);

    // Nº 16
    dx = 0.;
    dy = 0.;
    dz = z3;
    TGeoRotation *pMatrix217 = new TGeoRotation();
    pMatrix217->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix216 = new TGeoCombiTrans("", dx, dy, dz, pMatrix217);

    // Nº 4
    dx = 0.;
    dy = 0.;
    dz = z1;
    TGeoRotation *pMatrix205 = new TGeoRotation();
    pMatrix205->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix204 = new TGeoCombiTrans("", dx, dy, dz, pMatrix205);

    // Nº 26
    dx = 0.;
    dy = 0.;
    dz = z6;
    TGeoRotation *pMatrix227 = new TGeoRotation();
    pMatrix227->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix226 = new TGeoCombiTrans("", dx, dy, dz, pMatrix227);

    // Nº 18
    dx = 0.;
    dy = 0.;
    dz = z4;
    TGeoRotation *pMatrix219 = new TGeoRotation();
    pMatrix219->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix218 = new TGeoCombiTrans("", dx, dy, dz, pMatrix219);

    // Nº 14
    dx = 0.;
    dy = 0.;
    dz = z5;
    TGeoRotation *pMatrix215 = new TGeoRotation();
    pMatrix215->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix214 = new TGeoCombiTrans("", dx, dy, dz, pMatrix215);

    // Nº 28
    dx = 0.;
    dy = 0.;
    dz = z7;
    TGeoRotation *pMatrix229 = new TGeoRotation();
    pMatrix227->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix228 = new TGeoCombiTrans("", dx, dy, dz, pMatrix229);

    // Nº 30
    dx = 0.;
    dy = 0.;
    dz = z8;
    TGeoRotation *pMatrix231 = new TGeoRotation();
    pMatrix231->SetAngles(0, 0, 0);
    TGeoCombiTrans *pMatrix230 = new TGeoCombiTrans("", dx, dy, dz, pMatrix231);

    // Si sensors
    pWorld->AddNode(TraLog, 1, pMatrix204);
    pWorld->AddNode(TraLog, 2, pMatrix202);
    pWorld->AddNode(TraLog, 3, pMatrix216);
    pWorld->AddNode(TraLog, 4, pMatrix218);
    pWorld->AddNode(TraLog, 5, pMatrix214); // Tracker
    pWorld->AddNode(TraLog, 6, pMatrix226);
    pWorld->AddNode(TraLog, 7, pMatrix228);
    pWorld->AddNode(TraLog, 8, pMatrix230);

    // ---------------   Finish   -----------------------------------------------
    gGeoMan->CloseGeometry();
    gGeoMan->CheckOverlaps(0.001);
    gGeoMan->PrintOverlaps();
    gGeoMan->Test();

    TFile *geoFile = new TFile(geoFileName, "RECREATE");
    top->Write();
    top->Draw();
    geoFile->Close();
    std::cout << "\033[34m Creating geometry:\033[0m "
              << "\033[33m" << geoFileName << " \033[0m" << std::endl;
    // gApplication->Terminate();
}
