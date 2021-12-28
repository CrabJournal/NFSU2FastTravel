#pragma once

#include "R_math.h"

// Structs for NFSU, not really garantied yet that it the same for U2


struct RigidBody;
struct Player;
struct PhysicsMover;
struct RealDriver;

struct Car;
struct CarState;

struct DriverInfo;
struct CarTypeInfo;
struct RideInfo;

struct CarCollisionBody;

struct Body {
	Body *next;
	Body *prev;
	char mbtypeName[8];
};

struct unkObj_rawEntity {
	void *Vtable;
	unkObj_rawEntity* next;
	unkObj_rawEntity* prev;
	char mbtypeName[8];
};

// size 0xE8, 
struct DriveTrain : unkObj_rawEntity {
	float unk0x14;					// +0x14 =1.0
	BYTE unk0x18[0x68];
	float mbRedZoneStarts[7];		// +0x80
	// +0xE0 ? =0
};

// inh of Body obj vtbl 0x6C8584 - pure

// allocated in block size - 0x19000
// 1) size 0x1F0, vtlb 0x6B9654, typeName "Rigid"
// 2) size 0x190, vtlb 0x6B9544, typeName "Wheel"
struct RigidBody /* old name Object3D */ : unkObj_rawEntity {
	// void *Vtable;
	// 7: CalcSpeedmul or something

	BYTE unk0x14[0xC];						// + 0x14 / dseg*

	vector3a position;						// + 0x20
	float RotationMatrix4x4[4][4];			// + 0x30

	vector3a velocity;						// + 0x70
	vector3a unk0x80;						// + 0x80
	float unk0x90;							// + 0x90, mb time elapsed*??
	BYTE align0x94[0xC];
	vector3a velocity_multiplied;			// + 0xA0
	vector3a mbAceleration1;				// + 0xB0
	vector3a unk0xC0;						// + 0xC0 
	float matrix0xD0[3][4];					// + 0xD0	/ almost same as Rotation matrix
	vector3a unk0x100;						// + 0x100
	vector3a unk0x110;						// + 0x110
	BYTE unk0x120[0x10];					// + 0x120
	DWORD flag_mb0x130;						// + 0x130
	DWORD flag_mb0x134;						// + 0x134
	DWORD flag_mb0x138;						// + 0x138
	BYTE unk0x13C[0x8];						// + 0x13C

	float magic3;							// + 0x144 / number, than devide speed =1.22
	float magic3_inversed;					// + 0x148
	DWORD align0x14C;
	float unk0x150[0x10];					// + 0x150
	float unkMatrix0x190[4][4];				// + 0x190
	// +0x1E8 float
};

struct RideInfo {
	DWORD CarType; // 0 - GOLF, 8 = RX-7 FD, 0x13 - Skyline R34 GTR?
	// +0x14 arr of objs size 0x30
	// 0x60 obj1 / mostly floats, 
	// obj1 + 0x2A0 float (1)
	// obj1 + 0x2A4 int (6)
	// +0x424 BYTE
	// +0x434 - BYTE 
	// +0x64B - obj?
};

// global addr 0x77C790 (for player I guess)
// size 0x750
struct DriverInfo {
	BYTE mbFlag;
	BYTE unk0x2[2];				// +0x1
	BYTE mbNumberOfDriver;		// +0x3
	int unk0x4;					// +0x4  mb control mode (1 for player, 2 for AI)
	BYTE unk0x8[8];
	RideInfo rideinfo;			// +0x10		

	// +0x734 - BYTE mb control mode
};

// size 0xC90
struct CarTypeInfo {
	char Name[32];
	char Name_for_bin_key[32];			// +0x20
	char geometry_bin_path[32];			// +0x40
	char geometry_lzc_path[32];			// +0x60
	BYTE unk0x80[64];					// +0x80
	char Manufacturer[16];				// +0xC0
	DWORD BinKey;
	// +0xB60 num of PlayerCameras
	// +0xB60 - CameraSettings PlayerCameras[5]
	// chek binary for additional information, GlobasB.lzc (not only), 0x19DDE0 - offset for RX7
};

// size 0x13C
struct mbRacerInfo {
	Car* car;
	// not a single pointer anymore
	// +0x8 BYTE
	// +0x10 WORD
	// +0x14 float unk0x1C 
	BYTE unk0x4[0x138];
};


// old name WorldObject
struct Mover {
	// vtbl 5 entries
	virtual void* __thiscall Destroy(bool free);						// 0: +0x0
	virtual void __thiscall mbDoTimeStepMove(float TimeStep);			// 1: +0x4
	// ??, pos, rotation, speed
	virtual void __thiscall SaveInReplaySnapshot(void* ReplaySnapshot);	// 2: +0x8
	virtual RigidBody* __thiscall GetBody();							// 3: +0xC
	virtual DWORD __thiscall GetUnk();									// 4: +0x10
	// etc	
	// Mover_Vtbl* Vtable;		// 0x6CAADC - pure
	RealDriver* driver;		// +0x4
};

// size 0xC, vtbl 0x6CAAF0
struct NullMover : Mover {
	Car* car;//mb CarState?				// +0x8
};

// size 0x24 vtlb 0x6BA714, no RigidBody
struct TrafficMover : NullMover {
	float vector2_Pos[2];			// +0xC / RW
	float angle;					// +0x14 in radians mb
	float speed;					// +0x18  m/s
	DWORD unk0x1C[2];				// +0x1C / =0
};

/* Unknown obj size 0x2C, vtbl 0x6CAB08, from 0x42566C*/

// size 0x48, vtbl 0x6C89DC, no RigidBody
struct BeltMover : NullMover {
	DWORD unk0xC[2];				// +0xC =0
	void* unkobj0x14;				// +0x14, size 0x2C, no vtbl
	void* unkobj0x18;				// +0x18, size 0x30
	DWORD unk0x1C[3];				// +0x1C =0
	void* unkobj0x28;				// +0x28, size 0x60
	void* unkobj0x2C;				// +0x2C, size 0x60
	void* Average;					// +0x30, size 0x28
	BYTE unk0x34;					// +0x34 =0
	BYTE mbAlign0x35[3];			// +0x35 =BAADF0
	void* Wheels[4];				// +0x38 (for what?)
};

// double inhiritace i guess
// size 0x50 (in SlotPool), vtbl 0x6C8468
struct MellowMover : Mover {
	void* Vtlb;						// +0x8 some obj start here vtbl 0x6C845C (base 0x6C85A8) (3 entries) size 0x48
	char Name[8];					// +0xC
	void* Object3D_plus4;			// +0x14
	void* Object3D2_plus4;			// +0x18 Mover
	int unk;						// +0x1C
	RigidBody* body;				// +0x20
	void* Mellowator;				// +0x24
	void* Aerodynamics;				// +0x28
	void* dseg;						// +0x2C / 0x77CF50 floats
	CarState* state;				// +0x30
	Car* car;						// +0x34
	void* Wheels[4];				// +0x38
	DWORD unk0x48[2];				// +0x48 =0
};

/*	size 0x90 (vtlb 0x6C8364) */
struct PhysicsMover : Mover {				// old name: Worldobject
	void* mbVtable;					// +0x8 / 0x6C8358 (3 entries)
	char Name[8];					// +0xC / ="PhysMov"
	void* Object3D_plus4;			// +0x14
	void* Object3D2_plus4;			// +0x18 / DriveTrain + 4
	void* unk_dataseg0x1C;			// +0x1C / vtlb-0x6C8468, number 0x24, 32
	RigidBody* body;				// +0x20 / body or something / RigidBody
	void* TwoWheelAckermanSteering; // +0x24 size 0xA0
	void* Suspension[4];			// +0x28 size 0xC0
	void* Wheels[4];				// +0x38 size 0x190
	void* Engine;					// +0x48 size 0xC0
	DriveTrain* driveTrain;			// +0x4C size 0xE8
	void* Aerodynamics;				// +0x50 size 0x40
	void* dseg;						// +0x54
	CarState* carstate;				// +0x58
	Car* car;						// +0x5C
	// +0x78 float / =1.0
	// ect.
};



//size 0xA70, from 0x4222FF
struct Player {
	BYTE unk0x0[0x4];		// +0x0 / =0
	Car* car;				// +0x4
	BYTE unk0x8[0x8];
	DWORD playernumber;		// +0x10
	BYTE unk0x14[0x20];
	// +0x21 BYTE / =0
	char name[16];			// +0x34
	// +0xA8 - Average* / size 0x28
	// +0x108 pobj / size 0x30, from 0042BA25
	// +0x190 peModelSlopPool ??
	// +0x1AC float 5
	// +0x1B0 float 1
	// +0x1E0 same class pobj / size 0x90
	// +0x270 BYTE mb Nos flag idk
	// +0x27C - CameraMover
	// +0x28A - peModel maybe / size 0x140
	// +0x55C - pobj / size 0xA4
	// +0x5C0 - pobj / size 0x200
	// +0x914 int WorldTimer (start ?)
	// +0x95C - HUD ptr / size 0x90
};

// size ??
struct CarDriver {
	void* vtbl;						// +0x0 33 entries
	/*
	0:  Destroy
	2:  SaveInReplaySnapshot
	9:  GetGearCode
	11: SetGearCode
	17: GetDriverNumber
	*/
};

/*	RealDriver -	size 0x2C0, vtbl 0x6C8AD0, from 0x59C011 - player
	PsychoDriver -	size 0x250, vtbl 0x6C8C18, from 0x46218B - AI Racer,
	CarDriver -		size ??, vtbl 0x6CAA58
	DragDriver -	size 0x520, vtbl 0x6C8B90 (DragDriver + 0x250 - RealDriver)
	AIDriver -		size 0x500, vtbl 0x6C89F0
	0x6B94F0 */
struct RealDriver {
	void* Vtable;				// 
	void* mbCarLoader;			// +0x4 size 0x140, from 0x42B4EE, Car* for AIDriver
	void* unk_obj0x8;			// +0x8 size mb 8, allocated in block (size 0x4E20A8)
	void* unk_obj0xC;			// +0xC addr same as prev + 8

	// size 0x1C0 some buffer or something
	int unknnum;				// +0x10 / =0xA mb driver number
	DWORD mbAlign[3];			// +0x1C / =0
	vector3a vecs[10];			// +0x20 / idk why so much, seem like pos, vector3a.align = 0
	DWORD unk0xC0[40];			// +0xC0 / =0
	DWORD unk0x160[28];			// +0x160 / random shit

	DWORD unk0x1D0;				// mb actual car steering
	DWORD GearCode_RW;			// +0x1D4 0 - R, 1 - N, 2 - 1, etc.
	BYTE unk0x1D8[0x18];

	// some object starts here
	Car* car_p1;				// +0x1F0
	BYTE unk0x1F4[4];
	BYTE AveragUnk0x1F8[0X34];	// +0x1F8 AverageWindow (size 0x28) Vtable, so there may be AverageWindow object
	// +0x208 void* unk_obj size 0x84, from 0x05841B9 (Average::???)
	float unk0x22C;
	float unk0x230;

	BYTE unk0x234[0x2C];

	WORD actual_steering;		// +0x260
	BYTE unk0x262[0xA];

	Car* car_p2;				// +0x26C

	BYTE unk0x270[0x18];

	BYTE SteeringRight_R;		// +288
	BYTE mbAlign0x289[3];
	float Steering_RW;			// +0x28C 1 = Left, -1 = Right

	BYTE Gas_R;					// +0x290
	BYTE mbAlign0x291[3];
	float Gas_RW;// GAS GAS GAS	// +0x294 1 = full throttle

	BYTE Break_R;				// +0x298
	BYTE mbAlign0x299[3];
	float Break_RW;				// +0x29C

	BYTE unk0x2A0[8];

	BYTE E_Brake_R;				// +0x2A8
	BYTE mbAlign0x2A9[3];
	float E_Break_RW;			// +0x2AC
	// 16 bytes left
};

// 0x80
struct Collision {
	void *punk0;				// +0x0 World +0x10
	void *punk1;				// +0x4 =punk0
	DWORD mb_timers[2];			// +0x8 =0x0000F348,
	DWORD unk0x10[4];			// +0x10 =0
	vector3a mbCollisionPoint;	// +0x20
	vector3a same0x30;			// +0x30
	DWORD unk0x40[4];			// +0x40 =0
	vector3a unkv3;				// +0x50
	float module_of_some_vector;// +0x60
	DWORD mbAlign[3];			// +0x64
	vector3a ImpulseNormal;		// +0x70
};

// size 0x40 in slotpool
struct Contact {
	void* punk0;				// +0x0 stack
	void* punk1;				// +0x4 =punk0
	float timeaftercontactmb;	// +0x8 =0.0113932
	CarCollisionBody* collbody1;// +0xC
	void* collbody2;			// +0x10 TrackPolygonCollisionBody for example
	void* CollisionBodyWitness;	// +0x14

};

// struct DynamicCollisionBody : CollisionBody {};

// TrackPolygonCollisionBody size - 0x270
// WorldObjectCollisionBody, size 0x410, vtbl 0x6B8D88
// size 0x190, all cars traffic including have this
struct CarCollisionBody /*: DynamicCollisionBody */ {
	virtual void* __thiscall Destroy(bool free);							// 0:  +0x0
	virtual void __thiscall SaveInReplaySnapshot(void* ReplaySnapshot);		// 1:  +0x4
	virtual vector3* __thiscall GetPosition();								// 2:  +0x8
	virtual vector3* __thiscall GetVelocityMultiplied();					// 3:  +0xC
	virtual void __thiscall unkVF0x4();										// 4:  +0x10
	virtual float* __thiscall GetRotationMatrix4x4();						// 5:  +0x14
	virtual vector3a* __thiscall GetmbAceleration();						// 6:  +0x18
	virtual void __thiscall SetPosition(vector3* pos);						// 7:  +0x1C
	virtual float __thiscall GetSomeFloat();								// 8:  +0x20
	virtual float __thiscall GetmbTimeElapsed();							// 9:  +0x24
	virtual void __thiscall DoTimestep(float time);							// 10: +0x28

	// 29 entries
	//void *Vtable;					// 0x6C8798, size 0x190 

	CarCollisionBody* next;			// +0x4, ptr + 4 (no vtlb)
	CarCollisionBody* prev;			// +0x8
	void* unk0x8;					// +0xC data seg - 0x7377F0
	BYTE unk0x10[6];
	bool bunk0x16;					// +0x16 mbIsMovable?
	bool unkflag0x17;				// +0x17 

	int unk0x18;					// +0x18 =7
	DWORD unk0x1C;					// +0x1C =0x100
	DWORD unk0x20;					// +0x20 =0
	float mbVector4[4];				// +0x24 seems like a position
	char Name[8];					// +0x34 "Car0", "Car51" etc.

	BYTE unk0x3C[0x24];				// +0x3C =0

	CarState* state;				// +0x60 / null mostly / or just state or something idk / or 1 / isHaveRigidBody
	DWORD unk0x64;
	void* OBJ_in_mover;				// +0x68 / that obj with vtlb 0x6C845C in WorldObject + 8

	RigidBody* body;				// +0x6C

	float mbTimeElaplsed;			// +0x70
	DWORD mbAligment0x74[3];

	vector3a Normals[3];			// +0x80

	float unk0xB0[0x24];

	Car* car;						// +0x140
	// +0x80 Vector3;
	// +0xE8 float
	// +0xC0 vector3
	// +0x110 vector3
	// +0120 Matrix4x4 / what???
	// +0x140 DWORD
};

struct CarState {
	void* RideInfoPlus0x60;		// initial value
	BYTE unk0x4[0xC];
	vector3a somePos;			// +0x10
	vector3a somePos2;			// +0x20 / RW? / for camera
	// +0x30 - max engine torque 
	vector3a unk0x30;
	float unk0x40[3];			// +0x40
	BYTE unk0x4C[4];
	float Matrix4x4_0x50[4][4];	// +0x50 returns by CarCollisionBody::GetRotationMatrix4x4
	vector3a mbAceleration;		// +0x90
	float unk0xA0[4];
	float Matrix4x4_0xB0[4][4];	// +0xB0

	BYTE unk0xF0[0x284];
	// +0x1B0 obj_size0x80[4];	// wut??
	// +0x2C0 float unk[??];
	WORD angle1_R;				// +0x374 = TrafficMover.angle * 10430.378
	WORD unk0x376;				// +0x376
	BYTE unk0x378[0x1C];
	// +0x38C DWORD mbMovementMode
	// +0x390 float mbPrevSpeed mps R

	float speed_mps_R;			// +0x394 meters per second, for HUD
	DWORD GearCode_R;			// +0x398 for HUD
	DWORD IsClutch_R;			// +0x39C for HUD
	float RPM_R;				// +0x3A0 for HUD
	BYTE unk0x3A4[0x10];		// +0x3A4
	DWORD Nos1;					// +0x3B4
	DWORD Nos2;					// +0x3B8
	void* NosTimer;				// +0x3BC

	BYTE unk0x3C0[0x18];		// +0x3C0
	DWORD mbControlMode;		// +0x3D8
	// +0x420 - BYTE
};

enum CarMovmentMode {
	Null = 0,
	Physics = 1,
	Traffic = 2,
	Mellow = 3,
	Belt = 4
};
// AI's car allocated from  	004224D6
// player's from 				00421F7D
// size 0x8F0
struct Car {
	void* not_sure;					// next Car or World or ultra base object idk 
	void* not_sure2;				// too  
	BYTE unk1[0x2];
	short driver_number;			// +0xA / (or car number)
	CarTypeInfo* cartypeinfo;		// +0xC
	RideInfo* rideinfo;				// +0x10
	DriverInfo* driverinfo;			// +0x14
	Player* player;					// +0x18
	mbRacerInfo* mbracerinfo;		// +0x1C
	void* unk0x20;					// +0x20  / initial value / dataseg
/*	1/1 - player,					RealDriver / Physics Mover
	2/1 - player					DragDriver / Physics Mover
	3/1 - AI racer					PsychoDriver / Physics Mover
	3/3 - AI racer					Mellow Mover
	3/4 - AI racer					BeltMover
	4/0 - traffic car				Null Mover
	4/2 - traffic car				PsychoDriver (wtf) / Traffic Mover
	4/3 - traffic car with invalid *mover / Mellow mover what the fuck?	*/
	int DriverType;					// +0x24 / =1 for player 
	CarMovmentMode MovmentMode;		// +0x28 / =1 for player, 3 for AI, 4 if AI out of view / mb CurrentMovementMode
	void* unkObj0x2C;				// +0x2C / size 0x6C from 0x426040 / (AI without world object) size 0x500 from 0x42608E / unkObj2
	RealDriver* Driver;				// +0x30
	PhysicsMover* mover;			// +0x34 / or CarWorldObject / sometimes 0, sometimes object size 0x48 / 0x10F89E00 insted for Traffic car
	DWORD unk0x38;					// +0x38 / =0,2; on collision = 1
	CarCollisionBody* collision;	// +0x3C - 0 or some value sometimes
	CarState state;					// +0x40 // size 0x421 as minimum
	// 
	// +0x404 float / =1.0
	// +0x478 float
	// +0x47C float
	// +0x4D0 BYTE
	// +0x4D1 BYTE
	// +0x540 obj / size may be 0x2C0, no vtbl
	// +0x7F0 TrackPolygon* in SlotPool, size 0x80
	// +0x800 TrackPathCoordinate
	// +0x818 float
	// +0x828 BYTE
	// +0x82C obj ptr, size 0xA0, from 0x57A1D0
	// +0x868 obj ptr (mb Loader) / size 0xF8, 
	// +0x86C byte (bool?)
	// +0x878 EAXAITunerCar*, size 0x1FE0, from 0x51D70F
	// +0x8A0 Vector2?
};