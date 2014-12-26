
#include "mob.h"
#include "renderer.h"
#include "mob_manager.h"

extern Renderer gRenderer;
extern MobManager gMobMgr;

Mob::Mob(uint32 index, WLDSkeleton* skele, MobPosition* pos, WLDSkeleton* head) :
	mIndex(index),
	mHeadNode(nullptr),
	mRightHandNode(nullptr),
	mLeftHandNode(nullptr),
	mShieldNode(nullptr)
{
	init(skele, pos, head);
	setName("test");
}

Mob::Mob(Spawn_Struct* spawn, WLDSkeleton* skele, MobPosition* pos, WLDSkeleton* head) :
	mIndex(spawn->spawnId),
	mHeadNode(nullptr),
	mRightHandNode(nullptr),
	mLeftHandNode(nullptr),
	mShieldNode(nullptr)
{
	init(skele, pos, head);
	setName(spawn->name);

	mNode->setRotation(core::vector3df(0.0f, Util::EQ19toFloat(spawn->heading) / 256.0f * 360.0f, 0.0f));
}

void Mob::init(WLDSkeleton* skele, MobPosition* pos, WLDSkeleton* head)
{
	mSkeletonWLD = new WLDSkeletonInstance(gRenderer.copyMesh(skele->getReferenceMesh()), skele, this);
	mSkeletonWLD->assumeBasePosition();

	scene::IAnimatedMesh* mesh = new scene::SAnimatedMesh(mSkeletonWLD->getMesh());
	scene::ISceneManager* sceneMgr = gRenderer.getSceneManager();

	mNode = sceneMgr->addAnimatedMeshSceneNode(mesh, nullptr, -1, core::vector3df(pos->x, pos->y, pos->z));
	mesh->drop();

	if (skele->hasHeadPoint())
	{
		mHeadNode = sceneMgr->addEmptySceneNode(mNode); //make this a camera scene node later
	}

	if (head)
	{
		mHeadSkeleWLD = new WLDSkeletonInstance(gRenderer.copyMesh(head->getReferenceMesh()), head, this);
		mHeadSkeleWLD->assumeBasePosition();

		mesh = new scene::SAnimatedMesh(mHeadSkeleWLD->getMesh());

		sceneMgr->addAnimatedMeshSceneNode(mesh, mNode);
		mesh->drop();
	}
	else
	{
		mHeadSkeleWLD = nullptr;
	}

}

Mob::~Mob()
{
	if (mSkeletonWLD)
		delete mSkeletonWLD;
	if (mHeadSkeleWLD)
		delete mHeadSkeleWLD;
	mNode->removeAll();
}

void Mob::setName(const char* name)
{
	if (strlen(name) == 0)
		return;
	snprintf(mName, 64, "%s", name);
	core::stringw wide_name(mName);
	//should be placed above the HEAD_POINT node rather than the base node
	//default font is pretty terrible
	gRenderer.getSceneManager()->addBillboardTextSceneNode(nullptr, wide_name.c_str(), mNode,
		core::dimension2df(5.0f, 2.0f), core::vector3df(0, 5.0f, 0));
}

void Mob::animate(float delta)
{
	if (mSkeletonWLD)
	{
		mSkeletonWLD->animate(delta);
		if (mHeadSkeleWLD)
			mHeadSkeleWLD->animate(delta);

		//check point node position and rotation changes
		//head doesn't rotate
	}
}

void Mob::startAnimation(std::string id)
{
	if (mSkeletonWLD)
	{
		mSkeletonWLD->setAnimation(id);
		if (mHeadSkeleWLD)
			mHeadSkeleWLD->setAnimation(id);
	}
}

void Mob::updatePosition(MobPositionUpdate_Struct* update)
{
	//ignoring deltas / interpolation for now...
	core::vector3df pos(
		Util::EQ19toFloat(update->y_pos),
		Util::EQ19toFloat(update->z_pos),
		Util::EQ19toFloat(update->x_pos)
	);
	gMobMgr.getMobPosition(mIndex)->set(pos);
	mNode->setPosition(pos);
	mNode->setRotation(core::vector3df(0.0f, Util::EQ19toFloat(update->heading) / 256.0f * 360.0f - 180.0f, 0.0f));
}
