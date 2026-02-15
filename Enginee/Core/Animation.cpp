#include"Animation.h"
#include"RenderQueue.h"

void AnimatedSprite::Update(float dt) {
	if (!playing || !clip || clip->frames.empty())return;
	time += dt;
	const float frameDur = 1.0f / clip->fps;
	while (time >= frameDur) {
		time -= frameDur;
		frameIdx = (frameIdx + 1) % (int)clip->frames.size();
	}
}

void AnimatedSprite::Draw(RenderQueue& rq,int z) {
	if (!clip)return;
	SDL_Rect dst = {
		static_cast<int>(x),
		static_cast<int>(y),
		clip->frames[frameIdx].w,
		clip->frames[frameIdx].h
	};
	rq.Add({
		clip->tex,dst,clip->frames[frameIdx],z
		});

}
