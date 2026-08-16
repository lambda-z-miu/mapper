/*
 *    Copyright 2012, 2013 Thomas Schöps
 *    Copyright 2012-2020 Kai Pastor
 *
 *    This file is part of OpenOrienteering.
 *
 *    OpenOrienteering is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    OpenOrienteering is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with OpenOrienteering.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <clocale>
#include <memory>
#include <utility>
// IWYU pragma: no_include <type_traits>

#include <Qt>
#include <QtGlobal>
#include <QtPlugin>  // IWYU pragma: keep
#include <QApplication>
#include <QCoreApplication>
#include <QFontInfo>
#include <QGuiApplication>
#include <QLatin1String>
#include <QList>
#include <QLocale>
#include <QObject>
#include <QPointer>
#include <QSettings>
#include <QStaticPlugin>  // IWYU pragma: keep
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QTranslator>
#include <QWidget>

#ifdef MAPPER_USE_QTSINGLEAPPLICATION
#include <QtSingleApplication>
#include <QFileInfo>
#endif

#include "global.h"
#include "mapper_config.h"
#include "mapper_resource.h"
#include "gui/home_screen_controller.h"
#include "gui/main_window.h"
#include "gui/widgets/mapper_proxystyle.h"
#include "util/recording_translator.h"  // IWYU pragma: keep
#include "util/translation_util.h"

// IWYU pragma: no_forward_declare QTranslator

using namespace OpenOrienteering;
namespace {

void applyModernDesktopTheme()
{
	qApp->setStyleSheet(QStringLiteral(R"(
		QMainWindow { background: #f4f7fa; }
		#modernWindowChrome { background: #0d2538; }
		#modernWindowTitleBar { background: #0d2538; border-bottom: 1px solid #29475b; }
		#modernWindowTitle { color: #edf6fb; font-size: 15px; font-weight: 600; }
		#modernWindowTitleBar QToolButton { background: transparent; border: 0; border-radius: 0; color: #edf6fb; font-size: 19px; min-width: 44px; min-height: 38px; padding: 0; }
		#modernWindowTitleBar QToolButton:hover { background: #23495f; border: 0; }
		#modernWindowTitleBar QToolButton[windowControl="close"]:hover { background: #bd3345; color: #ffffff; }
		QMainWindow::separator { background: #081b29; width: 6px; height: 6px; }
		QMenuBar { background: #0d2538; color: #edf6fb; border-bottom: 1px solid #29475b; padding: 3px 12px; }
		QMenuBar::item { background: transparent; border-radius: 6px; margin: 2px 3px; padding: 5px 8px; }
		QMenuBar::item:selected { background: #23465d; color: #ffffff; }
		QMenu { background: #123247; color: #edf6fb; border: 1px solid #36566c; border-radius: 8px; padding: 6px; }
		QMenu::item { border-radius: 5px; padding: 8px 32px 8px 12px; }
		QMenu::item:selected { background: #23617f; color: #ffffff; }
		QToolBar { background: #102b3e; color: #edf6fb; border: 0; border-bottom: 1px solid #29495e; spacing: 4px; padding: 5px 8px; }
		QToolBar::separator { background: #3e5c70; margin: 7px 6px; width: 1px; }
		QToolButton { background: transparent; border: 1px solid transparent; border-radius: 7px; padding: 5px; }
		QToolButton:hover { background: #23495f; border-color: #47758d; }
		QToolButton:checked { background: #087ca8; border-color: #59c4e5; }
		QToolBar[modernRole="leftRail"] { background: #0f2a3d; border: 0; border-right: 1px solid #26465c; spacing: 6px; padding: 9px 7px; }
		QToolBar[modernRole="leftRail"]::separator { background: #3d5b70; height: 1px; margin: 7px 3px; }
		QToolBar[modernRole="leftRail"] QToolButton { background: transparent; border-color: transparent; }
		QToolBar[modernRole="leftRail"] QToolButton:hover { background: #22475f; border-color: #426a84; }
		QToolBar[modernRole="leftRail"] QToolButton:checked { background: #087ca8; border-color: #54bde0; }
		QDockWidget { background: #102b3e; color: #edf6fb; font-weight: 600; }
		QDockWidget::title { background: #102b3e; color: #edf6fb; border-bottom: 1px solid #29495e; padding: 9px 12px; text-align: left; }
		QDockWidget::close-button, QDockWidget::float-button { background: transparent; border-radius: 5px; padding: 3px; }
		QDockWidget::close-button:hover, QDockWidget::float-button:hover { background: #e8f2f7; }
		QStatusBar { background: #0d2538; color: #e6f1f7; border-top: 1px solid #29495e; font-size: 14px; min-height: 28px; padding: 4px 12px; }
		QStatusBar::item { border: 0; }
		QPushButton { background: #ffffff; border: 1px solid #c8d7e2; border-radius: 6px; color: #193248; padding: 6px 12px; }
		QPushButton:hover { background: #edf5f9; border-color: #8dbfd6; }
		QPushButton:default { background: #087ca8; border-color: #087ca8; color: white; }
		QComboBox, QLineEdit, QSpinBox { background: #ffffff; border: 1px solid #cbd9e3; border-radius: 6px; padding: 4px 7px; }
		QTabBar::tab { background: transparent; border: 0; border-bottom: 2px solid transparent; color: #b9cbd7; padding: 8px 12px; }
		QTabBar::tab:selected { color: #ffffff; border-bottom-color: #4cc8ed; }
		QScrollArea, SymbolRenderWidget { background: #102b3e; border: 0; }
		QDockWidget > QWidget, SymbolWidget, SymbolWidget QWidget { background: #102b3e; color: #edf6fb; }
		SymbolRenderWidget { background: #102b3e; }
		TemplateListWidget { background: #102b3e; color: #edf6fb; font-size: 15px; }
		TemplateListWidget QTableView { background: #102b3e; alternate-background-color: #173a50; color: #edf6fb; border: 1px solid #36566c; gridline-color: #29495e; font-size: 15px; }
		TemplateListWidget QTableView::item { padding: 6px; }
		TemplateListWidget QTableView::item:selected { background: #23617f; color: #ffffff; }
		TemplateListWidget QHeaderView::section { background: #173a50; color: #dcecf5; border: 0; border-bottom: 1px solid #36566c; padding: 7px; font-size: 14px; }
		QDockWidget QLineEdit, QDockWidget QComboBox, QDockWidget QSpinBox { background: #173a50; color: #edf6fb; border-color: #41687d; }
		QDockWidget QPushButton { background: #173a50; color: #edf6fb; border-color: #41687d; }
		QDockWidget QPushButton:hover { background: #23617f; border-color: #59c4e5; }
		QScrollBar:vertical { background: #102b3e; width: 12px; margin: 2px; }
		QScrollBar::handle:vertical { background: #46677b; border-radius: 5px; min-height: 28px; }
		QScrollBar::handle:vertical:hover { background: #5f90a7; }
		QStatusBar QLabel, QStatusBar QFrame, QStatusBar QToolButton { color: #edf6fb; font-size: 14px; }
		QStatusBar QComboBox, QStatusBar QLineEdit { background: #173a50; color: #edf6fb; border-color: #41687d; }
		SymbolWidget { border-left: 6px solid #081b29; border-right: 6px solid #081b29; }	)"));
}

}  // namespace


#if defined(MAPPER_USE_FAKE_POSITION_PLUGIN)
Q_IMPORT_PLUGIN(FakePositionPlugin)
#endif

#if defined(Q_OS_WIN) && defined(MAPPER_USE_POWERSHELL_POSITION_PLUGIN)
Q_IMPORT_PLUGIN(PowershellPositionPlugin)
#endif

#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS)) && defined(MAPPER_USE_NMEA_POSITION_PLUGIN)
Q_IMPORT_PLUGIN(NmeaPositionPlugin)
#endif

#if defined(SCALING_ICON_ENGINE_PLUGIN)
Q_IMPORT_PLUGIN(ScalingIconEnginePlugin)
#endif


// From map.h

namespace OpenOrienteering {

extern QPointer<QTranslator> map_symbol_translator;

}  // namespace OpenOrienteering


QStringList firstRemoved(QStringList&& input)
{
	if (!input.empty())
		input.removeFirst();
	return std::move(input);
}


#ifdef MAPPER_USE_QTSINGLEAPPLICATION

void resetActivationWindow(QtSingleApplication& app)
{
	const auto* const old_window = app.activationWindow();
	app.setActivationWindow(nullptr);
	
	if (!QCoreApplication::closingDown())
	{
		const auto top_level_widgets = QApplication::topLevelWidgets();
		for (auto* widget : top_level_widgets)
		{	
			auto* const new_window = qobject_cast<MainWindow*>(widget);
			if (new_window && new_window != old_window)
			{
				app.setActivationWindow(new_window);
				QObject::connect(new_window, &QObject::destroyed, &app, [&app]() { resetActivationWindow(app); });
				QObject::connect(&app, &QtSingleApplication::messageReceived, new_window, &MainWindow::openPathLater);
				break;
			}
		}
	}
}

#endif


int main(int argc, char** argv)
{
#ifdef MAPPER_USE_QTSINGLEAPPLICATION
	// Create single-instance application.
	// Use "oo-mapper" instead of the executable as identifier, in case we launch from different paths.
	QtSingleApplication qapp(QString::fromLatin1("oo-mapper"), argc, argv);
	if (qapp.isRunning()) {
		// Send a message to activate the running app, and optionally open a file
		auto const arguments = firstRemoved(QCoreApplication::arguments());
		for (auto const& arg : arguments)
			qapp.sendMessage(QFileInfo(arg).absoluteFilePath());
		return 0;
	}
#else
	QApplication qapp(argc, argv);
#endif
	
#ifdef Q_OS_ANDROID
	qputenv("QT_USE_ANDROID_NATIVE_STYLE", "1");
#endif
	
	// Load resources
	Q_INIT_RESOURCE(resources);
	
	// QSettings on OS X benefits from using an internet domain here.
	QCoreApplication::setOrganizationName(QString::fromLatin1("OpenOrienteering.org"));
	QCoreApplication::setApplicationName(QString::fromLatin1("Mapper"));
	QGuiApplication::setApplicationDisplayName(APP_NAME + QString::fromUtf8(" " APP_VERSION));
	
#ifdef WIN32
	// Load plugins on Windows
	QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + QLatin1String("/plugins"));
#endif
	
	MapperResource::setSeachPaths();
	
	// Localization
	QSettings settings;
	TranslationUtil::setBaseName(QLatin1String("OpenOrienteering"));
	TranslationUtil translation(settings);
	QLocale::setDefault(QLocale(translation.code()));
#if defined(Q_OS_MACOS)
	// Normally this is done in Settings::apply() because it is too late here.
	// But Mapper 0.6.2/0.6.3 accidentally wrote a string instead of a list. This
	// error caused crashes when opening native dialogs (i.e. the open-file dialog!).
	settings.setValue(QString::fromLatin1("AppleLanguages"), QStringList{ translation.code() });
#endif
#if defined(Mapper_DEBUG_TRANSLATIONS)
	if (!translation.getAppTranslator().isEmpty())
	{
		// Debug translation only if there is a Mapper translation, i.e. not for English.
		QCoreApplication::installTranslator(new RecordingTranslator());
	}
#endif
	QCoreApplication::installTranslator(&translation.getQtTranslator());
	QCoreApplication::installTranslator(&translation.getAppTranslator());
	map_symbol_translator = translation.load(QString::fromLatin1("map_symbols")).release();
	if (map_symbol_translator)
		map_symbol_translator->setParent(&qapp);
	
	// Avoid numeric issues in libraries such as GDAL
	setlocale(LC_NUMERIC, "C");
	
	// Initialize static things like the file format registry.
	doStaticInitializations();
	
	// Some style settings (in particular the menu item font) are not
	// applied correctly before the app runs. So we postpone these steps
	// via the event loop.
	// OTOH the app crashes on Android if we don't set style early enough.
	QTimer::singleShot(0, qApp, [&qapp]() {
#ifndef __clang_analyzer__
		// No leak: QApplication takes ownership.
		QApplication::setStyle(new MapperProxyStyle());
#endif
		applyModernDesktopTheme();
		
		auto ui_font = QApplication::font();
		if (ui_font.pointSizeF() > 0)
			ui_font.setPointSizeF(1.1 * ui_font.pointSizeF());
		else
			ui_font.setPixelSize(qRound(1.1 * QFontInfo(ui_font).pixelSize()));
		QApplication::setFont(ui_font);

		// Create first main window
		auto first_window = new MainWindow();
		Q_ASSERT(first_window->testAttribute(Qt::WA_DeleteOnClose));
		first_window->setController(new HomeScreenController());
		
		// Open given files later, i.e. after the initial home screen has been
		// displayed. In this way, error messages for missing files will show on 
		// top of a regular main window (home screen or other file).
		
		// Treat all program parameters as files to be opened
		auto const arguments = firstRemoved(QCoreApplication::arguments());
		for (auto const& arg : arguments)
			first_window->openPathLater(arg);
		
		first_window->applicationStateChanged();
		
#ifdef MAPPER_USE_QTSINGLEAPPLICATION
		resetActivationWindow(qapp);
#endif
		
		first_window->showMaximized();
		first_window->raise();
	});
	
	// Let application run
	return QApplication::exec();
}
