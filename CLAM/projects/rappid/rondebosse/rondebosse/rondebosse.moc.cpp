/****************************************************************************
** RondeBosse meta object code from reading C++ file 'rondebosse.h'
**
** Created: Wed Oct 16 11:25:04 2002
**      by: The Qt MOC ($Id$)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "rondebosse.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *RondeBosse::className() const
{
    return "RondeBosse";
}

QMetaObject *RondeBosse::metaObj = 0;
static QMetaObjectCleanUp cleanUp_RondeBosse;

#ifndef QT_NO_TRANSLATION
QString RondeBosse::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RondeBosse", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString RondeBosse::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "RondeBosse", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* RondeBosse::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = MainWindow::staticMetaObject();
    static const QUMethod slot_0 = {"OnbtnExitClick", 0, 0 };
    static const QUMethod slot_1 = {"OnbtnFileOpenClick", 0, 0 };
    static const QUMethod slot_2 = {"OnbtnFileOpenLeftClick", 0, 0 };
    static const QUMethod slot_3 = {"OnbtnFileSaveOutputClick", 0, 0 };
    static const QUMethod slot_4 = {"OnbtnFileOpenRightClick", 0, 0 };
    static const QUMethod slot_5 = {"OnbtnFileSaveClick", 0, 0 };
    static const QUMethod slot_6 = {"OnbtnHelpClick", 0, 0 };
    static const QUMethod slot_7 = {"OnbtnRecallClick", 0, 0 };
    static const QUMethod slot_8 = {"OnbtnStartClick", 0, 0 };
    static const QUMethod slot_9 = {"OnbtnStopClick", 0, 0 };
    static const QUMethod slot_10 = {"OnbtnStoreClick", 0, 0 };
    static const QUMethod slot_11 = {"OnchbRealTimeClick", 0, 0 };
    static const QUMethod slot_12 = {"OnchbWaveFileClick", 0, 0 };
    static const QUMethod slot_13 = {"OnrbRealTimeLeftClick", 0, 0 };
    static const QUMethod slot_14 = {"OnrbRealTimeRightClick", 0, 0 };
    static const QUMethod slot_15 = {"OnrbSynthLeftClick", 0, 0 };
    static const QUMethod slot_16 = {"OnrbSynthRightClick", 0, 0 };
    static const QUMethod slot_17 = {"OnrbWaveFileLeftClick", 0, 0 };
    static const QUMethod slot_18 = {"OnrbWaveFileRightClick", 0, 0 };
    static const QUMethod slot_19 = {"OnsldConfigMove", 0, 0 };
    static const QUMethod slot_20 = {"OnsldControlMove", 0, 0 };
    static const QUMethod slot_21 = {"OnsldLeft1Move", 0, 0 };
    static const QUMethod slot_22 = {"OnsldLeft2Move", 0, 0 };
    static const QUMethod slot_23 = {"OnsldLeft3Move", 0, 0 };
    static const QUMethod slot_24 = {"OnsldLeft4Move", 0, 0 };
    static const QUMethod slot_25 = {"OnsldRight1Move", 0, 0 };
    static const QUMethod slot_26 = {"OnsldRight2Move", 0, 0 };
    static const QUMethod slot_27 = {"OnsldRight3Move", 0, 0 };
    static const QUMethod slot_28 = {"OnsldRight4Move", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "OnbtnExitClick()", &slot_0, QMetaData::Public },
	{ "OnbtnFileOpenClick()", &slot_1, QMetaData::Public },
	{ "OnbtnFileOpenLeftClick()", &slot_2, QMetaData::Public },
	{ "OnbtnFileSaveOutputClick()", &slot_3, QMetaData::Public },
	{ "OnbtnFileOpenRightClick()", &slot_4, QMetaData::Public },
	{ "OnbtnFileSaveClick()", &slot_5, QMetaData::Public },
	{ "OnbtnHelpClick()", &slot_6, QMetaData::Public },
	{ "OnbtnRecallClick()", &slot_7, QMetaData::Public },
	{ "OnbtnStartClick()", &slot_8, QMetaData::Public },
	{ "OnbtnStopClick()", &slot_9, QMetaData::Public },
	{ "OnbtnStoreClick()", &slot_10, QMetaData::Public },
	{ "OnchbRealTimeClick()", &slot_11, QMetaData::Public },
	{ "OnchbWaveFileClick()", &slot_12, QMetaData::Public },
	{ "OnrbRealTimeLeftClick()", &slot_13, QMetaData::Public },
	{ "OnrbRealTimeRightClick()", &slot_14, QMetaData::Public },
	{ "OnrbSynthLeftClick()", &slot_15, QMetaData::Public },
	{ "OnrbSynthRightClick()", &slot_16, QMetaData::Public },
	{ "OnrbWaveFileLeftClick()", &slot_17, QMetaData::Public },
	{ "OnrbWaveFileRightClick()", &slot_18, QMetaData::Public },
	{ "OnsldConfigMove()", &slot_19, QMetaData::Public },
	{ "OnsldControlMove()", &slot_20, QMetaData::Public },
	{ "OnsldLeft1Move()", &slot_21, QMetaData::Public },
	{ "OnsldLeft2Move()", &slot_22, QMetaData::Public },
	{ "OnsldLeft3Move()", &slot_23, QMetaData::Public },
	{ "OnsldLeft4Move()", &slot_24, QMetaData::Public },
	{ "OnsldRight1Move()", &slot_25, QMetaData::Public },
	{ "OnsldRight2Move()", &slot_26, QMetaData::Public },
	{ "OnsldRight3Move()", &slot_27, QMetaData::Public },
	{ "OnsldRight4Move()", &slot_28, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"RondeBosse", parentObject,
	slot_tbl, 29,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_RondeBosse.setMetaObject( metaObj );
    return metaObj;
}

void* RondeBosse::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "RondeBosse" ) ) return (RondeBosse*)this;
    return MainWindow::qt_cast( clname );
}

bool RondeBosse::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: OnbtnExitClick(); break;
    case 1: OnbtnFileOpenClick(); break;
    case 2: OnbtnFileOpenLeftClick(); break;
    case 3: OnbtnFileSaveOutputClick(); break;
    case 4: OnbtnFileOpenRightClick(); break;
    case 5: OnbtnFileSaveClick(); break;
    case 6: OnbtnHelpClick(); break;
    case 7: OnbtnRecallClick(); break;
    case 8: OnbtnStartClick(); break;
    case 9: OnbtnStopClick(); break;
    case 10: OnbtnStoreClick(); break;
    case 11: OnchbRealTimeClick(); break;
    case 12: OnchbWaveFileClick(); break;
    case 13: OnrbRealTimeLeftClick(); break;
    case 14: OnrbRealTimeRightClick(); break;
    case 15: OnrbSynthLeftClick(); break;
    case 16: OnrbSynthRightClick(); break;
    case 17: OnrbWaveFileLeftClick(); break;
    case 18: OnrbWaveFileRightClick(); break;
    case 19: OnsldConfigMove(); break;
    case 20: OnsldControlMove(); break;
    case 21: OnsldLeft1Move(); break;
    case 22: OnsldLeft2Move(); break;
    case 23: OnsldLeft3Move(); break;
    case 24: OnsldLeft4Move(); break;
    case 25: OnsldRight1Move(); break;
    case 26: OnsldRight2Move(); break;
    case 27: OnsldRight3Move(); break;
    case 28: OnsldRight4Move(); break;
    default:
	return MainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool RondeBosse::qt_emit( int _id, QUObject* _o )
{
    return MainWindow::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool RondeBosse::qt_property( int _id, int _f, QVariant* _v)
{
    return MainWindow::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
