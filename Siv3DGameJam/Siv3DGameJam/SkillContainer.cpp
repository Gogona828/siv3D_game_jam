#include "SkillContainer.h"
// スキルのヘッダー追加
#include "FIX.h"
#include "BruteForce.h"
// ボス行動
#include "BossNormalAttack.h"

using namespace s3d;

SkillContainer& SkillContainer::getInstance() {
	static SkillContainer instance;
	return instance;
}

const SkillContainer::Entry* SkillContainer::table(size_t& outSize) {
	static const Entry t[] = {
		// このフォーマットでスキルを足していく
		{ U"FIX.atk",   &SkillContainer::Make<FIX>  },
		{ U"BruteForce.atk",   &SkillContainer::Make<BruteForce>  },
		// ボス用コマンド
		{ U"BossNormalAttack.atk",   &SkillContainer::Make<BossNormalAttack>  },
	};
	outSize = std::size(t);
	return t;
}

SkillContainer::CreatorFn SkillContainer::findCreator(const String& key) {
	size_t n = 0; const Entry* t = table(n);
	for (size_t i = 0; i < n; ++i) {
		if (key == t[i].key) return t[i].make;
	}
	return nullptr;
}

bool SkillContainer::registerSkill(const String& key) {
	if (!findCreator(key)) return false;
	m_registered.emplace(key);
	return true;
}

bool SkillContainer::unregisterSkill(const String& key) {
	return (m_registered.erase(key) > 0);
}

bool SkillContainer::isRegistered(const String& key) const {
	return m_registered.contains(key);
}

std::unique_ptr<ISkill> SkillContainer::createRegistered(const String& key) const {
	if (!m_registered.contains(key)) return nullptr;
	if (auto make = findCreator(key)) return make();
	return nullptr;
}

std::unique_ptr<ISkill> SkillContainer::createKnown(const String& key) const {
	if (auto make = findCreator(key)) return make();
	return nullptr;
}

Array<String> SkillContainer::knownKeys() const {
	size_t n = 0; const Entry* t = table(n);
	Array<String> out; out.reserve(n);
	for (size_t i = 0; i < n; ++i) out << String{ t[i].key };
	out.sort();
	return out;
}

Array<String> SkillContainer::registeredKeys() const {
	Array<String> out(m_registered.begin(), m_registered.end());
	out.sort();
	return out;
}
