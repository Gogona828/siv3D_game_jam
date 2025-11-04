#pragma once
#include <Siv3D.hpp>
#include <memory>
#include "ISkill.h"
using namespace s3d;

class SkillContainer {
public:
	static SkillContainer& getInstance();

	// キーだけで登録
	bool registerSkill(const s3d::String& key);

	// 解除 / 確認
	bool unregisterSkill(const s3d::String& key);
	bool isRegistered(const s3d::String& key) const;

	// ★ 生成：登録済みのキーからだけ作る（未登録なら nullptr）
	std::unique_ptr<ISkill> createRegistered(const s3d::String& key) const;

	// 生成：既知キーから作る（登録済みかは問わない）
	std::unique_ptr<ISkill> createKnown(const s3d::String& key) const;

	// ユーティリティ
	s3d::Array<s3d::String> knownKeys() const;       // 静的対応表のキー一覧
	s3d::Array<s3d::String> registeredKeys() const;  // 取得済みキー一覧

	using CreatorFn = std::unique_ptr<ISkill>(*)();
	struct Entry { s3d::StringView key; CreatorFn make; };
	const Entry* getSkillTable(size_t& key) { return table(key); }

private:
	SkillContainer() = default;

	// ==== 内部：静的対応表（キー→生成関数）====
	template<class T>
	static std::unique_ptr<ISkill> Make() { return std::make_unique<T>(); }

	// .cpp 側に実体（FixAtk.hpp 等を include した上で埋める）
	static const Entry* table(size_t& outSize);
	static CreatorFn findCreator(const s3d::String& key);

	// 取得済みスキル（キーのみ保持）
	s3d::HashSet<s3d::String> m_registered;
};
