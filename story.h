// story.h
#pragma once
#include <stdbool.h>

void story_play(const char* storyId, void(*storyFunc)(void));

void story_init(void);
bool story_has_seen(const char* id);
void story_mark_seen(const char* id);
