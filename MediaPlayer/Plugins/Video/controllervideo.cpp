#include "controllervideo.h"

void ControllerVideo::exec() {}

QString ControllerVideo::playerView() const {
	return "qrc:/video/VideoPlayer.qml";
}

QString ControllerVideo::playlistView() const {
	return "qrc:/video/VideoPlaylist.qml";
}

void ControllerVideo::setPlaylist(PlaylistPointer) {}

void ControllerVideo::setMedia(MediaPointer) {}

MediaRole ControllerVideo::role() const { return MediaRole::Video; }

QStringList ControllerVideo::filters() const {
	return {"ts", "mp4", "mkv", "avi"};
}
