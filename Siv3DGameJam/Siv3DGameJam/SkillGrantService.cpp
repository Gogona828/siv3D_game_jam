# include "SkillGrantService.h"

SkillGrantService& SkillGrantService::getInstance() {
	static SkillGrantService instance;
	return instance;
}
