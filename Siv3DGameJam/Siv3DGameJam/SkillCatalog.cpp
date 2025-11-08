#include "stdafx.h"
#include "SkillCatalog.h"
#include "Skills/HelloWorldSkill.h"
#include "Skills/LogicalBombSkill.h"
#include "Skills/ZeroDaySkill.h"
#include "Skills/InjectionSkill.h"
#include "Skills/RainbowAttackSkill.h"
#include "Skills/KillSkill.h"
#include "Skills/SilverBulletSkill.h"
#include "Skills/Raid5Skill.h"
#include "Skills/MFASkill.h"
#include "Skills/ChecksumSkill.h"
#include "Skills/FirewallSkill.h"
#include "Skills/SplitHorizonSkill.h"
#include "Skills/DefragSkill.h"
#include "Skills/GarbageCollectionSkill.h"
#include "Skills/RefactoringSkill.h"
#include "Skills/ReviseSkill.h"
#include "Skills/SelfRepairSkill.h"
#include "Skills/HoneypotSkill.h"
#include "Skills/CriticalChainSkill.h"
#include "Skills/ZeroTrustSkill.h"
#include "Skills/OverclockSkill.h"
#include "Skills/RollbackSkill.h"
#include "BossSkills/PhantomReadSkill.h"
#include "BossSkills/OverflowSkill.h"
#include "BossSkills/DeadlockSkill.h"
#include "BossSkills/DimensionalCompressionSkill.h"
#include "BossSkills/MagicNumberSkill.h"
#include "Skills/FIX.h"
#include "Skills/BruteForce.h"
#include "BossNormalAttack.h"

SkillCatalog::SkillCatalog()
{
}

SkillCatalog::~SkillCatalog()
{
}

bool SkillCatalog::load()
{
	m_skills.clear();

	// Manually add skills for now
	auto helloWorld = std::make_unique<HelloWorldSkill>();
	m_skills.emplace(helloWorld->getName(), std::move(helloWorld));

	auto logicalBomb = std::make_unique<LogicalBombSkill>();
	m_skills.emplace(logicalBomb->getName(), std::move(logicalBomb));

	auto zeroDay = std::make_unique<ZeroDaySkill>();
	m_skills.emplace(zeroDay->getName(), std::move(zeroDay));

	auto injection = std::make_unique<InjectionSkill>();
	m_skills.emplace(injection->getName(), std::move(injection));

	auto rainbowAttack = std::make_unique<RainbowAttackSkill>();
	m_skills.emplace(rainbowAttack->getName(), std::move(rainbowAttack));

	auto kill = std::make_unique<KillSkill>();
	m_skills.emplace(kill->getName(), std::move(kill));

	auto silverBullet = std::make_unique<SilverBulletSkill>();
	m_skills.emplace(silverBullet->getName(), std::move(silverBullet));

	auto raid5 = std::make_unique<Raid5Skill>();
	m_skills.emplace(raid5->getName(), std::move(raid5));

	auto mfa = std::make_unique<MFASkill>();
	m_skills.emplace(mfa->getName(), std::move(mfa));

	auto checksum = std::make_unique<ChecksumSkill>();
	m_skills.emplace(checksum->getName(), std::move(checksum));

	auto firewall = std::make_unique<FirewallSkill>();
	m_skills.emplace(firewall->getName(), std::move(firewall));

	auto splitHorizon = std::make_unique<SplitHorizonSkill>();
	m_skills.emplace(splitHorizon->getName(), std::move(splitHorizon));

	auto defrag = std::make_unique<DefragSkill>();
	m_skills.emplace(defrag->getName(), std::move(defrag));

	auto garbageCollection = std::make_unique<GarbageCollectionSkill>();
	m_skills.emplace(garbageCollection->getName(), std::move(garbageCollection));

	auto refactoring = std::make_unique<RefactoringSkill>();
	m_skills.emplace(refactoring->getName(), std::move(refactoring));

	auto revise = std::make_unique<ReviseSkill>();
	m_skills.emplace(revise->getName(), std::move(revise));

	auto selfRepair = std::make_unique<SelfRepairSkill>();
	m_skills.emplace(selfRepair->getName(), std::move(selfRepair));

	auto honeypot = std::make_unique<HoneypotSkill>();
	m_skills.emplace(honeypot->getName(), std::move(honeypot));

	auto criticalChain = std::make_unique<CriticalChainSkill>();
	m_skills.emplace(criticalChain->getName(), std::move(criticalChain));

	auto zeroTrust = std::make_unique<ZeroTrustSkill>();
	m_skills.emplace(zeroTrust->getName(), std::move(zeroTrust));

	auto overclock = std::make_unique<OverclockSkill>();
	m_skills.emplace(overclock->getName(), std::move(overclock));

	auto rollback = std::make_unique<RollbackSkill>();
	m_skills.emplace(rollback->getName(), std::move(rollback));

	auto phantomRead = std::make_unique<PhantomReadSkill>();
	m_skills.emplace(phantomRead->getName(), std::move(phantomRead));

	auto overflow = std::make_unique<OverflowSkill>();
	m_skills.emplace(overflow->getName(), std::move(overflow));

	auto deadlock = std::make_unique<DeadlockSkill>();
	m_skills.emplace(deadlock->getName(), std::move(deadlock));

	auto dimensionalCompression = std::make_unique<DimensionalCompressionSkill>();
	m_skills.emplace(dimensionalCompression->getName(), std::move(dimensionalCompression));

	auto magicNumber = std::make_unique<MagicNumberSkill>();
	m_skills.emplace(magicNumber->getName(), std::move(magicNumber));

	auto fix = std::make_unique<FIX>();
	m_skills.emplace(fix->getName(), std::move(fix));

	auto bruteForce = std::make_unique<BruteForce>();
	m_skills.emplace(bruteForce->getName(), std::move(bruteForce));

	auto bossAttack = std::make_unique<BossNormalAttack>();
	m_skills.emplace(bossAttack->getName(), std::move(bossAttack));

	// TODO: Load all other skills from the CSV

	return true;
}

const ISkill* SkillCatalog::find(const String& name) const
{
	if (auto it = m_skills.find(name); it != m_skills.end())
	{
		return it->second.get();
	}
	return nullptr;
}

const std::unordered_map<String, std::unique_ptr<ISkill>>& SkillCatalog::getSkills() const
{
	return m_skills;
}
