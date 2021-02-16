#include "controllerlibrary.h"

ControllerLibrary::ControllerLibrary() {
	auto *context = m_engine->qmlEngine().rootContext();
	context->setContextProperty("_libraries", this);
}

ControllerLibrary::ControllerLibrary(const ControllerLibrary &)
		: AbstractController() {}

void ControllerLibrary::exec() {}

Library *ControllerLibrary::currentLibrary() const {
	return m_currentLibrary.data();
}

void ControllerLibrary::open() {
	m_engine->createWindow(QUrl(QStringLiteral("/LibraryView.qml")));
}

void ControllerLibrary::addSourceDir(QString source) {
	m_currentLibrary->addSourceDir(source);
}

void ControllerLibrary::removeSourceDir(QString path) {
	m_currentLibrary->removeSourceDir(path);
}

void ControllerLibrary::onCurrentModelChanged(LibraryPointer p) {
	m_currentLibrary = p;

	if (p) {
		connect(p.data(), &Library::libraryChanged, this,
						&ControllerLibrary::onLibraryChanged);

		emit currentLibraryChanged();

		p->probe()->setFilters(m_manager[p->role()]->filters());
	}
}
void ControllerLibrary::onCurrentPlaylistChanged(PlaylistPointer p) {
	//	auto p = m_playlistModel.current();
	if (m_currentLibrary && !p.isNull() && m_manager[m_currentLibrary->role()])
		m_manager[m_currentLibrary->role()]->setPlaylist(p);
}

void ControllerLibrary::addPlaylist(bool smart) {
	if (smart) {
		auto pl = factory<SmartPlaylist>();
		m_currentLibrary->addSmartPlaylist(pl);
	} else {
		auto pl = factory<PlayList>();
		m_currentLibrary->addPlaylist(pl);
	}
}

void ControllerLibrary::removePlaylist(QString id) {
	m_currentLibrary->removePlaylist(id);
	m_currentLibrary->removeSmartPlaylist(id);
}

void ControllerLibrary::onLibraryChanged() {
	db()->updateLibrary(m_currentLibrary);
}
