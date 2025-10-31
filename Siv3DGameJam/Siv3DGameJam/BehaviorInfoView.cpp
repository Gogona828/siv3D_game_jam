# include "BehaviorInfoView.h"
using namespace s3d;

void BehaviorInfoView::draw() const
{
	RectF infoBackground( Arg::center(275, 230), 600, 50 );
	const Transformer2D t0{ Mat3x2::Translate(infoBackground.center()) };
	infoBackground.draw(ColorF{0});

	font(skillInfo).draw(18, Arg::center(infoBackground.center()));
}
