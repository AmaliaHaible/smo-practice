#pragma once

#include "al/collision/Triangle.h"
#include "al/rail/BezierCurve.h"
#include "al/rail/LinearCurve.h"
#include "debugMenu.hpp"
#include "fl/ui/ui.h"
#include <game/Player/PlayerActorHakoniwa.h>
#include <game/StageScene/StageScene.h>
#include <sead/container/seadPtrArray.h>
#include <sead/math/seadQuat.h>
#include <sead/math/seadVector.h>
#include <str.h>
#include <fl/input.h>

#define NUMSAVES 16
#define MAXDEPTH 16

namespace fl {

struct SaveState {
    bool mSaved = false;
    sead::Vector3f mTrans = { 0, 0, 0 };
    sead::Quatf mQuat = { 0, 0, 0, 0 };
};

namespace ui {

    class Page;

    constexpr int kMaxPathPoints = 1024;
    constexpr int kMaxCapThrows = 32;

    class PracticeUI {
    public:
        static inline PracticeUI& instance()
        {
            static PracticeUI ui;
            return ui;
        }

        void update(StageScene* stageScene);
#if (SMOVER == 100)
        void menu(sead::TextWriter& p);
#endif
#if (SMOVER == 130)
        void menu();
#endif
        void kill();

        inline StageScene* getStageScene()
        {
            return stageScene;
        }
        template <typename... Args>
        void printf(const char* format, Args... args)
        {
            printPos += sprintf(textBuffer + printPos, format, args...);
        }

        struct {
            bool teleportEnabled = true;
            bool noclipEnabled = false;
            bool shineRefresh = true;
            bool gotShineRefresh = true;
            bool alwaysWarp = true;
            bool disableAutoSave = true;
            bool skipBowser = false;
            bool buttonMotionRoll = false;
            bool moonJump = false;
            bool loadCurrentFile = false;
            bool loadFileConfirm = false;
            bool repeatCapBounce = false;
            bool repeatRainbowSpin = false;
            bool wallJumpCapBounce = false;
            bool disableCameraVertical = false;
            bool disableCameraStop = false;
            bool noDamageLife = false;
            bool lockHack = false;
            bool lockCarry = false;
            bool disableShineNumUnlock = false;
            bool showOddSpace = false;

            bool disablePuppet = false;
            bool overrideBowserHat0 = false;
            bool reloadDUP = false;
            bool shouldRender = true;
            bool muteBgm = false;

            bool pipeMazeOverride = false;
            int pipeMazeConfig[3] = { 0, 0, 0 }; // 2 pipes ; 3 pipes ; 5 pipes
        } options;

        struct {
            bool showPlayer = false;
            bool showAxis = false;
            bool showArea = false;
            bool showAreaPoint = false;
            bool showAreaGroup = false;
            bool showHitInfoFloor = false;
            bool showHitInfoWall = false;
            bool showHitInfoCeil = false;
            bool showHitInfoArray = false;
            bool showCRC = false;
            al::AreaObjGroup* curAreaGroup = nullptr;
            al::AreaObj* curArea = nullptr;
            sead::Vector3f nearestEdgePoint = sead::Vector3f::zero;
            sead::Vector3f actorTrans = sead::Vector3f::zero;
#if SMOVER == 100
            al::Triangle kclTri = al::Triangle();
#endif
            bool showHitSensors = false;

            // Trickjump review mode: shows Mario's (and optionally Cappy's) recorded path,
            // and while active swaps in noclip + no-gravity fly controls.
            bool reviewModeActive = false;
            sead::Vector3f marioPathPoints[kMaxPathPoints];
            int marioPathPointCount = 0; // number of valid points, saturates at kMaxPathPoints
            int marioPathWriteIndex = 0; // next write position (ring buffer)

            // Full trail of Cappy's flight, accumulated across every throw since the last reset.
            bool showCapFullPath = false;
            sead::Vector3f capPathPoints[kMaxPathPoints];
            int capPathPointCount = 0;
            int capPathWriteIndex = 0;

            // One entry per throw: where Cappy was thrown from and which way, kept across throws.
            bool showCapThrowMarkers = false;
            sead::Vector3f capThrowPositions[kMaxCapThrows];
            sead::Vector3f capThrowDirections[kMaxCapThrows];
            int capThrowCount = 0;
            int capThrowWriteIndex = 0;
        } renderer;

        struct {
            bool isModeDiverOrJungleGymRom = false;
            bool isModeDiverRom = false;
            bool isModeE3LiveRom = false;
            bool isModeE3MovieRom = false;
            bool isModeEpdMovieRom = false;
            bool isModeJungleGymRom = false;
        } modes;

        enum Page : u8 {
            Menu, About,
            Options, OptionsMvmt, OptionsMoon, OptionsSave, OptionsCamera, OptionsTeleport, OptionsRenderer, OptionsPipeMaze, OptionsReview,
            Stage, Misc,
            Info, InfoPlayer, InfoCapBounce, InfoMoon, InfoHack, InfoCappy, InfoStatistics, InfoCamera, InfoMighty1, InfoMighty2, InfoJump, InfoRelative,
            Tas, Modes, Debug,
            Test, TestWallAction, TestCapActionHistory,
            TestLiveActorKit, TestObjectList, TestAreaList,
            TestExecute, TestExecuteDraw,
            TestNatureDirector, TestCollisionDirector,
            TestGameDataHolder, TestGameDataFile, TestGameDataShine,
            TestTalkatoo, TestOther, TestLoadingZones
        };

        enum MofumofuPattern : s8 {
            Random = -1,
            Ghost = 0,
            Nose = 1,
            C = 2,
            W = 3,
            J = 4,
            Medal = 5,
            Plane = 6,
            Five = 7,
            Hangman = 8,
            Spanish = 9,
            Siblings = 10,
            Snake = 11,
            Eight = 12,
            Mushroom = 13,
            Z = 14,
            Tetris = 15,
            Ear = 16,
            Bomb = 17,
            Bird = 18,
            L = 19,
            O = 20,
            Star = 21
        };
        MofumofuPattern curPattern = Random;

        struct MofumofuPatternEntry {
            const char* typeStr;
            int target;
            bool reverse;
        };

        constexpr static const MofumofuPatternEntry mPatternEntries[22] = {
            { "Ghost", 0, false }, { "Nose", 0, true }, { "C", 1, false }, { "W", 1, true }, { "J", 2, false }, { "Medal", 2, true }, { "Plane", 3, false }, { "5", 3, true }, { "Hangman", 4, false }, { "Spanish", 4, true },
            { "Siblings", 5, false }, { "Snake", 5, true }, { "8", 6, false }, { "Mushroom", 6, true }, { "Z", 7, false }, { "Tetris", 7, true }, { "Ear", 8, false }, { "Bomb", 8, true }, { "Bird", 9, false }, { "L", 9, true }, { "O", 10, false }, { "Star", 10, true }
        };

        Page curPage = Page::Menu;
        s8 curLine = 0;
        u8 maxLine = 0;
        bool inputEnabled = true;
        bool hideShineCounter = false;
        bool nextFrameNoLeftInput = false;
        bool nextFrameNoRightInput = false;

        s32 actorIndex = 0;
        s32 areaGroupIndex = 0;
        s32 areaIndex = 0;
        s32 dataShineIdx = 0;
        s8 executeTableDrawIdx = 0;
        s32 executeListIdx = 0;
        s32 executeItemIdx = 0;
        s32 itemIdx = 0;
        u8 wallAction = 0;
        s8 otherPageIdx = 0;
        int talkatooIndex = 0;
        int curWorldId = 0;
        int curScenarioNo = 1;
        s8 changeStageNum = 0;
        s8 curDepth = 0;
        s8 cursorHistory[MAXDEPTH] = { 0 };
        bool movingPage = false;
        bool quatRot = false;
        bool observedVelocities = true;
        bool sphericalVel = false;
        bool deltaVel = false;
        bool absoluteTAS = false;
        al::LinearCurve* linearCurve = nullptr;
        al::BezierCurve* bezierCurve = nullptr;
        al::LiveActor* currentActor;
        s8 numDigits = 3;

        bool testDrawCube = false;
        bool testDrawCylinder = false;
        bool testDrawSphere = false;

        struct LiveActorInfo {
            sead::Vector3f trans, vel, accel, decay;
            float hSpeed, speed, hVelAngle, vVelAngle;
            sead::Vector2f hVel, deltaHVel, deltaHDecay;
            sead::Vector3f sphericalVel, deltaSphericalVel;
            sead::Quatf* quat;
            sead::Vector3f eulerAngles;
            sead::Vector3f rot, angVel, angAccel;
            const char* anim;
            float animFrame, animFrameMax;
            #if SMOVER == 100
            const char* nerveName;
            int nerveStep;
            #endif

            //double relativeAngle = 0;
            //sead::Vector3f relativeTrans = sead::Vector3f::zero;

            LiveActorInfo(al::LiveActor* actor, bool isPlayer, bool observedVelocities, const LiveActorInfo& old);
            LiveActorInfo() {}
        };

        struct CameraInfo {
            sead::Vector3f at, pos, up, diff;
            float dist;
            sead::Vector3f vel;
            float hSpeed, speed, hVelAngle, vVelAngle;
            sead::Vector2f hVel, deltaHVel;
            sead::Vector2f ang, angVel, angAccel; //(H, V)

            CameraInfo(const CameraInfo& old);
            CameraInfo() {}
        };

        struct InputInfo {
            sead::Vector2f left, right;
            sead::Vector2f leftPolar, rightPolar;
            sead::Vector2f leftPolarAbsolute; //the actual direction being pointed to in game
            bool a, b, x, y, l, r, zl, zr, plus, minus, ls, rs, dup, ddown, dleft, dright;

            InputInfo(const InputInfo& old);
            InputInfo() {};
        };

    private:
        StageScene* stageScene { nullptr };
        u8 curMaxLine = 0;
        SaveState savestates[NUMSAVES];
        s8 savestateIndex = 0;
        bool noclipping;
        s8 reloadStageForPos = -1;
        sead::Vector3f reloadStageTrans;
        sead::Quatf reloadStageQuat;

        float moonJumpVel = 50.0f;
        char* demangleBuffer = static_cast<char*>(malloc(64));
        size_t demangledSize;
        int cxaStatus;
        al::LiveActor* currentHack;
        al::HitSensor* currentCarry;
        int heldDirFrames = 0;

        bool reviewModeWasActive = false; // for detecting the on/off transition to (re)apply noclip/gravity
        bool capWasFlyingLastFrame = false; // for detecting the start of a new throw
        int capThrowButtonWindow = 0; // frames left where a recent X/Y press still counts as "just threw"
        sead::Vector3f savedGravity = sead::Vector3f::zero; // Mario's gravity, saved while review mode zeroes it

        char textBuffer[4096];
        u32 printPos;

        void savePosition(PlayerActorHakoniwa& player, s8 idx);
        void loadPositionPlayer(PlayerActorHakoniwa& player, s8 idx);
        void loadPosition(al::LiveActor* actor, s8 idx);
        void toggleNoclip(PlayerActorHakoniwa& player);
        void resetMarioPath();
        void updateMarioPath(PlayerActorHakoniwa& player);
        void resetCapPath();
        void updateCapPath(PlayerActorHakoniwa& player, HackCap* cap);
        void updateReviewModeFlight(PlayerActorHakoniwa& player);
        #if SMOVER == 100
        const char* getNerveName(al::LiveActor* actor);
        #endif
    };

} // namespace ui
} // namespace fl
