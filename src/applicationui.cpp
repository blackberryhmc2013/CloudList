/* Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/Container>
#include <bb/cascades/Button>
#include <bb/cascades/TextField>
#include <unistd.h>

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);
    //test_list = new MediaDB<std::string>();

    Container* cont = (Container*) root->children().at(0);
    // TODO... findChild by id didn't seem to work
    list = (ListView*) cont->children().at(0);
    txt = (TextField*) cont->children().at(1);
    if(list) {
    	data_list = new bb::cascades::QListDataModel<QVariantMap>();
    	list->setDataModel(data_list);
    } else {
    	qDebug() << "no list found?";
    }

    cltest = new CloudList<QString>("TemplateTest");
    qml -> setContextProperty("myApp", this);

    connect(txt->input(), SIGNAL(submitted(bb::cascades::AbstractTextControl*)), this, SLOT(submitData()) );
    connect(cltest, SIGNAL(newObject(int)) , this, SLOT(onItemAdded(int)));
    connect(cltest, SIGNAL(deletedObject(int)) , this, SLOT(onItemDeleted(int)));
    connect(cltest, SIGNAL(updatedObject(int)), this, SLOT(onItemChange(int)) );
}

void ApplicationUI::onItemAdded(int index)
{
	QVariantMap item;
	item["updateTime"] = cltest->lastUpdate(index).toString("MMMM d");
	item["body"] = cltest->at(index);
	item["id"] = cltest->lastUpdate(index).toString("h:mm:ss ap");
	data_list -> append(item);
}

void ApplicationUI::deleteItem(int index)
{
	cltest->remove(index);
}

void ApplicationUI::onItemDeleted(int index)
{
	data_list -> removeAt(index);
}

void ApplicationUI::submitData() {
	cltest -> add(txt->text());
	txt->resetText();
}

void ApplicationUI::onItemChange(int index)
{
	//data_list.replace(index, cltest->at(index) );
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("cloudList_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
