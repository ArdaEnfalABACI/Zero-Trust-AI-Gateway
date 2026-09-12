#include <QApplication>
#include <QCoreApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxyFactory>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QScrollBar>
#include <QKeyEvent>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QtSql>
#include <QSysInfo> 
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QProgressBar>
#include <QPointer>
#include <QScrollArea>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

// Applies modern Dark/Light theme styles via CSS
void applyModernTheme(QApplication& app, bool isDark) {
    QString css = isDark ? R"(
        QWidget { background-color: #0e0e0e; color: #e3e3e3; font-family: 'Segoe UI', Arial, sans-serif; font-size: 14px; }
        QPushButton { background-color: #1e1e1e; border: 1px solid #333333; border-radius: 8px; padding: 8px 16px; font-weight: bold; }
        QPushButton:hover { background-color: #333333; border: 1px solid #444444; }
        QPushButton:pressed { background-color: #444444; }
        
        QPushButton#primaryBtn { background-color: #3b82f6; color: #ffffff; border: none; }
        QPushButton#primaryBtn:hover { background-color: #60a5fa; border: none; }
        QPushButton#primaryBtn:disabled { background-color: #1e3a8a; color: #9ca3af; border: none; }
        
        QPushButton#sendBtn { background-color: #3b82f6; color: #ffffff; border: none; border-radius: 18px; font-size: 20px; font-weight: bold; padding: 0px; }
        QPushButton#sendBtn:hover { background-color: #60a5fa; border: none; }
        QPushButton#sendBtn:disabled { background-color: transparent; color: #3f3f46; border: none; } 
        
        QPushButton#cancelButton { background-color: #ef4444; color: white; border: none; border-radius: 18px; font-size: 18px; padding: 0px; }
        QPushButton#cancelButton:hover { background-color: #dc2626; border: none; }
        QPushButton#cancelButton:pressed { background-color: #b91c1c; border: none; }
        
        QPushButton#toggleSidebarBtn { background-color: transparent; border: 1px solid #333333; border-radius: 8px; font-size: 18px; padding: 4px; }
        QPushButton#toggleSidebarBtn:hover { background-color: #333333; }

        QWidget#inputContainer { background-color: #1e1e1e; border: 1px solid #333333; border-radius: 22px; }
        QTextEdit#inputField { background: transparent; border: none; color: #e3e3e3; font-size: 15px; }
        
        QLineEdit, QListWidget, QComboBox { background-color: #1e1e1e; border: 1px solid #333333; border-radius: 8px; padding: 8px; }
        QLineEdit:focus, QComboBox:focus { border: 1px solid #3b82f6; }
        QComboBox::drop-down { border: none; }
        QScrollBar:vertical { background: #0e0e0e; width: 10px; margin: 0px; }
        QScrollBar::handle:vertical { background: #333333; min-height: 20px; border-radius: 5px; }
        QScrollBar::handle:vertical:hover { background: #444444; }
        QGroupBox { border: 1px solid #333333; border-radius: 8px; margin-top: 20px; font-weight: bold; padding-top: 15px; }
        QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 5px; color: #3b82f6; }
        QListWidget::item:selected { background-color: #1e3a8a; border-radius: 5px; }
        QListWidget::item:hover { background-color: #333333; border-radius: 5px; }
        
        QScrollArea { border: none; background-color: transparent; }
        QWidget#chatContainerWidget { background-color: transparent; }
    )" : R"(
        QWidget { background-color: #ffffff; color: #0f0f0f; font-family: 'Segoe UI', Arial, sans-serif; font-size: 14px; }
        QPushButton { background-color: #f0f4f9; border: 1px solid #e4e4e7; border-radius: 8px; padding: 8px 16px; font-weight: bold; }
        QPushButton:hover { background-color: #e4e4e7; }
        QPushButton:pressed { background-color: #d4d4d8; }
        
        QPushButton#primaryBtn { background-color: #0b57d0; color: white; border: none; }
        QPushButton#primaryBtn:hover { background-color: #2563eb; border: none; }
        QPushButton#primaryBtn:disabled { background-color: #bfdbfe; color: #60a5fa; border: none; }
        
        QPushButton#sendBtn { background-color: #0b57d0; color: white; border: none; border-radius: 18px; font-size: 20px; font-weight: bold; padding: 0px; }
        QPushButton#sendBtn:hover { background-color: #2563eb; border: none; }
        QPushButton#sendBtn:disabled { background-color: transparent; color: #a1a1aa; border: none; }
        
        QPushButton#cancelButton { background-color: #ef4444; color: white; border: none; border-radius: 18px; font-size: 18px; padding: 0px; }
        QPushButton#cancelButton:hover { background-color: #dc2626; border: none; }
        QPushButton#cancelButton:pressed { background-color: #b91c1c; border: none; }
        
        QPushButton#toggleSidebarBtn { background-color: transparent; border: 1px solid #e4e4e7; border-radius: 8px; font-size: 18px; padding: 4px; }
        QPushButton#toggleSidebarBtn:hover { background-color: #f0f4f9; }

        QWidget#inputContainer { background-color: #f0f4f9; border: 1px solid #e4e4e7; border-radius: 22px; }
        QTextEdit#inputField { background: transparent; border: none; color: #0f0f0f; font-size: 15px; }
        
        QLineEdit, QListWidget, QComboBox { background-color: #f0f4f9; border: 1px solid #e4e4e7; border-radius: 8px; padding: 8px; }
        QLineEdit:focus, QComboBox:focus { border: 1px solid #0b57d0; }
        QComboBox::drop-down { border: none; }
        QScrollBar:vertical { background: #ffffff; width: 10px; margin: 0px; }
        QScrollBar::handle:vertical { background: #d4d4d8; min-height: 20px; border-radius: 5px; }
        QScrollBar::handle:vertical:hover { background: #a1a1aa; }
        QGroupBox { border: 1px solid #e4e4e7; border-radius: 8px; margin-top: 20px; font-weight: bold; padding-top: 15px; }
        QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; padding: 0 5px; color: #0b57d0; }
        QListWidget::item:selected { background-color: #bfdbfe; border-radius: 5px; }
        QListWidget::item:hover { background-color: #e4e4e7; border-radius: 5px; }
        
        QScrollArea { border: none; background-color: transparent; }
        QWidget#chatContainerWidget { background-color: transparent; }
    )";
    app.setStyleSheet(css);
}

// Basic encryption using machine ID to secure local DB content
QString secureObfuscate(const QString& input, bool encrypt) {
    QByteArray data = encrypt ? input.toUtf8() : QByteArray::fromBase64(input.toUtf8());
    
    QByteArray rawKey = QSysInfo::machineUniqueId(); 
    if (rawKey.isEmpty()) rawKey = "failsafe_key_for_zero_trust_v2";
    
    QByteArray hashedKey = QCryptographicHash::hash(rawKey, QCryptographicHash::Sha256);
    
    for (int i = 0; i < data.size(); ++i) { 
        data[i] = data[i] ^ hashedKey[i % hashedKey.size()]; 
    }
    return encrypt ? QString(data.toBase64()) : QString::fromUtf8(data);
}

// Retrieves a setting value from SQLite
QString getSetting(const QString& key) {
    QSqlQuery q; 
    q.prepare("SELECT value FROM settings WHERE key = :key"); 
    q.bindValue(":key", key);
    return (q.exec() && q.next()) ? secureObfuscate(q.value(0).toString(), false) : "";
}

// Saves a setting value to SQLite
void setSetting(const QString& key, const QString& value) {
    QSqlQuery q; 
    q.prepare("INSERT OR REPLACE INTO settings (key, value) VALUES (:key, :value)");
    q.bindValue(":key", key); 
    q.bindValue(":value", secureObfuscate(value, true)); 
    q.exec();
}

// Initializes SQLite database in the AppData directory to avoid permission issues
void setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;
    if (!dir.exists(dataDir)) dir.mkpath(dataDir);
    
    db.setDatabaseName(dataDir + "/zerotrust_history.sqlite");
    if (db.open()) {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS sessions (id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT, created_at DATETIME DEFAULT CURRENT_TIMESTAMP)");
        query.exec("CREATE TABLE IF NOT EXISTS messages (id INTEGER PRIMARY KEY AUTOINCREMENT, session_id INTEGER, role TEXT, content TEXT, model_info TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS settings (key TEXT PRIMARY KEY, value TEXT)");
    }
}

// Checks if the user has saved at least one API key
bool hasAnyApiKey() {
    return !getSetting("openai_api_key").isEmpty() || 
           !getSetting("anthropic_api_key").isEmpty() || 
           !getSetting("gemini_api_key").isEmpty();
}

// Structure to hold chat data in memory
struct ChatMessage {
    enum Type { System, User, AI, Error };
    Type type; 
    QString content; 
    QString modelInfo;
};

// Event filter to handle 'Enter' key for sending messages
class EnterSubmitFilter : public QObject {
public:
    std::function<void()> onSubmit;
    EnterSubmitFilter(QObject *parent = nullptr) : QObject(parent) {}
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                if (keyEvent->modifiers() & Qt::ShiftModifier) return false; // Shift+Enter for new line
                if (onSubmit) onSubmit(); 
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }
};

int main(int argc, char *argv[]) {
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    QApplication app(argc, argv);
    setupDatabase();
    QNetworkAccessManager networkManager;

    // Show welcome dialog on first launch to collect an API key
    if (!hasAnyApiKey()) {
        applyModernTheme(app, true);
        QDialog welcomeDialog;
        welcomeDialog.setWindowTitle("Zero Trust AI - Initial Setup");
        welcomeDialog.setFixedSize(450, 260);
        auto *wl = new QVBoxLayout(&welcomeDialog);
        auto *titleLbl = new QLabel("<h3 style='color:#3b82f6; margin:0;'>Welcome to Universal Zero Trust</h3><p>Please enter at least one API Key to begin.</p>");
        titleLbl->setAlignment(Qt::AlignCenter);
        
        auto *provCombo = new QComboBox(); 
        provCombo->addItems({"Gemini", "OpenAI", "Anthropic"});
        auto *keyInput = new QLineEdit(); 
        keyInput->setEchoMode(QLineEdit::PasswordEchoOnEdit); 
        keyInput->setPlaceholderText("Paste your API Key here...");
        
        auto *saveBtn = new QPushButton("Save & Start"); 
        saveBtn->setObjectName("primaryBtn"); 
        saveBtn->setFixedHeight(45);
        
        wl->addWidget(titleLbl); wl->addSpacing(10); wl->addWidget(provCombo); 
        wl->addWidget(keyInput); wl->addSpacing(20); wl->addWidget(saveBtn);
        
        QObject::connect(saveBtn, &QPushButton::clicked, [&]() {
            QString k = keyInput->text().trimmed();
            if (k.isEmpty()) { QMessageBox::warning(&welcomeDialog, "Error", "API Key cannot be empty."); return; }
            setSetting(provCombo->currentText().toLower() + "_api_key", k); 
            welcomeDialog.accept();
        });
        if (welcomeDialog.exec() != QDialog::Accepted) return 0; 
    }

    QWidget window;
    window.setWindowTitle("Zero Trust Gateway");
    window.resize(1300, 850);

    auto *mainLayout = new QHBoxLayout(&window);
    mainLayout->setContentsMargins(10, 10, 10, 10); 
    mainLayout->setSpacing(15);

    // Sidebar UI setup
    auto *sidebarWidget = new QWidget();
    sidebarWidget->setFixedWidth(260);
    auto *sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(0,0,0,0);
    
    auto *newChatBtn = new QPushButton("➕ New Chat");
    newChatBtn->setFixedHeight(45);
    auto *sessionList = new QListWidget();
    auto *settingsBtn = new QPushButton("⚙️ Settings");
    settingsBtn->setFixedHeight(40);
    auto *deleteChatBtn = new QPushButton("🗑️ Delete Chat");
    deleteChatBtn->setFixedHeight(40);
    deleteChatBtn->setDisabled(true); 

    sidebarLayout->addWidget(newChatBtn);
    sidebarLayout->addWidget(sessionList);
    sidebarLayout->addWidget(deleteChatBtn);
    sidebarLayout->addWidget(settingsBtn);

    // Main chat area UI setup
    auto *chatWidget = new QWidget();
    auto *chatLayout = new QVBoxLayout(chatWidget);
    chatLayout->setContentsMargins(0,0,0,0);
    
    auto *topLayout = new QHBoxLayout();
    auto *toggleSidebarBtn = new QPushButton("☰");
    toggleSidebarBtn->setObjectName("toggleSidebarBtn");
    toggleSidebarBtn->setFixedSize(38, 38);
    toggleSidebarBtn->setCursor(Qt::PointingHandCursor);
    QObject::connect(toggleSidebarBtn, &QPushButton::clicked, [&]() {
        sidebarWidget->setVisible(!sidebarWidget->isVisible());
    });

    auto *incognitoCheck = new QCheckBox("Incognito"); 
    auto *providerCombo = new QComboBox(); 
    providerCombo->addItems({"gemini", "openai", "anthropic"}); 
    providerCombo->setFixedWidth(130);
    
    auto *modelCombo = new QComboBox(); 
    modelCombo->setFixedWidth(230);
    
    auto updateModelsList = [modelCombo](const QString &provider) {
        modelCombo->clear();
        if (provider == "openai") {
            modelCombo->addItems({"gpt-4o", "gpt-4o-mini", "gpt-4-turbo", "gpt-3.5-turbo", "o1-preview", "o1-mini"});
        } else if (provider == "anthropic") {
            modelCombo->addItems({"claude-3-5-sonnet-20240620", "claude-3-opus-20240229", "claude-3-haiku-20240307"});
        } else if (provider == "gemini") {
            modelCombo->addItems({"gemini-3.8-flash", "gemini-3.5-flash-lite", "gemini-1.5-pro", "gemini-1.5-flash"});
        }
    };
    QObject::connect(providerCombo, &QComboBox::currentTextChanged, updateModelsList);

    QString lastProv = getSetting("last_provider");
    if (!lastProv.isEmpty()) providerCombo->setCurrentText(lastProv);
    else providerCombo->setCurrentText("gemini");
    updateModelsList(providerCombo->currentText());

    auto *themeCombo = new QComboBox();
    themeCombo->addItems({"Dark Mode", "Light Mode"});
    themeCombo->setFixedWidth(120);

    topLayout->addWidget(toggleSidebarBtn);
    topLayout->addSpacing(10);
    topLayout->addWidget(incognitoCheck);
    topLayout->addSpacing(15);
    topLayout->addStretch();
    topLayout->addWidget(new QLabel("Engine:")); topLayout->addWidget(providerCombo);
    topLayout->addWidget(new QLabel("Model:")); topLayout->addWidget(modelCombo);
    topLayout->addSpacing(15);
    topLayout->addWidget(themeCombo);
    chatLayout->addLayout(topLayout);

    // Chat scroll area setup
    auto *chatScrollArea = new QScrollArea();
    chatScrollArea->setWidgetResizable(true);
    chatScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    auto *chatContainerWidget = new QWidget();
    chatContainerWidget->setObjectName("chatContainerWidget");
    auto *chatMessagesLayout = new QVBoxLayout(chatContainerWidget);
    chatMessagesLayout->setContentsMargins(20, 20, 30, 20); 
    chatMessagesLayout->setSpacing(25); 
    chatMessagesLayout->addStretch(); 
    
    chatScrollArea->setWidget(chatContainerWidget);
    
    // User input area setup
    auto *inputWrapperLayout = new QHBoxLayout(); 
    inputWrapperLayout->setContentsMargins(50, 0, 50, 10); 

    auto *inputContainer = new QWidget();
    inputContainer->setObjectName("inputContainer");
    inputContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); 
    
    auto *containerLayout = new QHBoxLayout(inputContainer);
    containerLayout->setContentsMargins(15, 8, 8, 8); 
    containerLayout->setSpacing(10);

    auto *inputField = new QTextEdit();
    inputField->setObjectName("inputField");
    
    // Auto-resize input field based on text length
    int minInputHeight = 36; 
    int maxInputHeight = 150; 
    inputField->setFixedHeight(minInputHeight); 
    inputField->setDisabled(true); 
    
    QObject::connect(inputField, &QTextEdit::textChanged, [inputField, minInputHeight, maxInputHeight]() {
        QSizeF docSize = inputField->document()->size();
        int newHeight = static_cast<int>(docSize.height()) + 10; 
        if (newHeight < minInputHeight) newHeight = minInputHeight;
        if (newHeight > maxInputHeight) newHeight = maxInputHeight; 
        inputField->setFixedHeight(newHeight);
    });

    auto *btnLayout = new QVBoxLayout();
    btnLayout->addStretch(); 
    
    auto *sendButton = new QPushButton("↑"); 
    sendButton->setObjectName("sendBtn"); 
    sendButton->setDisabled(true); 
    sendButton->setFixedSize(36, 36); 
    sendButton->setCursor(Qt::PointingHandCursor);
    
    auto *cancelButton = new QPushButton("■"); 
    cancelButton->setObjectName("cancelButton"); 
    cancelButton->setFixedSize(36, 36); 
    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->hide();

    btnLayout->addWidget(sendButton);
    btnLayout->addWidget(cancelButton);
    
    containerLayout->addWidget(inputField);
    containerLayout->addLayout(btnLayout);
    inputWrapperLayout->addWidget(inputContainer);
    
    chatLayout->addWidget(chatScrollArea, 1);
    chatLayout->addLayout(inputWrapperLayout, 0);

    mainLayout->addWidget(sidebarWidget);
    mainLayout->addWidget(chatWidget);

    std::vector<ChatMessage> chatHistory;
    int currentSessionId = -1; 
    QNetworkReply *activeReply = nullptr;

    // Dynamically renders the chat history on the screen
    auto renderChatHistory = [&]() {
        // Clear previous widgets to prevent memory leaks
        QLayoutItem* item;
        while ((item = chatMessagesLayout->takeAt(0)) != nullptr) {
            if (item->widget()) { item->widget()->deleteLater(); }
            delete item;
        }

        bool isDark = (themeCombo->currentText() == "Dark Mode");
        QString mainText = isDark ? "#e3e3e3" : "#0f0f0f";
        QString userBg = isDark ? "#27272a" : "#e4e4e7"; 
        QString aiNameColor = isDark ? "#a8c7fa" : "#0b57d0"; 

        for (const auto& msg : chatHistory) {
            QWidget *rowWidget = new QWidget();
            QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(0, 0, 0, 0);
            
            if (msg.type == ChatMessage::System) {
                if (!msg.content.startsWith("DLP_REPORT|")) {
                    QLabel *sysLbl = new QLabel(msg.content);
                    sysLbl->setStyleSheet(QString("color: %1; font-weight: bold; font-size: 14px;").arg(aiNameColor));
                    sysLbl->setAlignment(Qt::AlignCenter);
                    rowLayout->addWidget(sysLbl);
                }
            } 
            else if (msg.type == ChatMessage::User) {
                QLabel *bubble = new QLabel(msg.content.toHtmlEscaped().replace("\n", "<br>"));
                bubble->setWordWrap(true);
                bubble->setTextInteractionFlags(Qt::TextBrowserInteraction);
                bubble->setStyleSheet(QString(
                    "background-color: %1; color: %2; padding: 12px 18px; border-radius: 20px; font-size: 15px;"
                ).arg(userBg, mainText));
                
                bubble->setMaximumWidth(800); 
                rowLayout->addStretch(); 
                rowLayout->addWidget(bubble);
            } 
            else if (msg.type == ChatMessage::AI) {
                QWidget *aiBlock = new QWidget();
                QVBoxLayout *aiLayout = new QVBoxLayout(aiBlock);
                aiLayout->setContentsMargins(0, 0, 0, 0);
                aiLayout->setSpacing(8);
                
<<<<<<< HEAD
                QLabel *header = new QLabel(QString("<b style='color:%1; font-size: 16px;'>AI Assistant </b><span style='color:#71717a; font-size:12px;'>(%2)</span>").arg(aiNameColor, msg.modelInfo));
=======
                QLabel *header = new QLabel(QString("<b style='color:%1; font-size: 16px;'> AI Assistant </b><span style='color:#71717a; font-size:12px;'>(%2)</span>").arg(aiNameColor, msg.modelInfo));
>>>>>>> 0617fffb2ffe89227b5e2e83ad30f878da6bd2c9
                
                QLabel *body = new QLabel(msg.content.toHtmlEscaped().replace("\n", "<br>"));
                body->setWordWrap(true);
                body->setTextInteractionFlags(Qt::TextBrowserInteraction);
                body->setStyleSheet(QString("color: %1; font-size: 15px;").arg(mainText));
                
                aiLayout->addWidget(header);
                aiLayout->addWidget(body);
                
                rowLayout->addWidget(aiBlock);
                rowLayout->addStretch(); 
            }
            else if (msg.type == ChatMessage::Error) {
                QLabel *errLbl = new QLabel(QString(" Gateway Error:<br>%1").arg(msg.content.toHtmlEscaped()));
                errLbl->setWordWrap(true);
                errLbl->setStyleSheet("background-color: rgba(239, 68, 68, 0.1); color: #EF4444; padding: 15px; border-left: 4px solid #EF4444; border-radius: 8px; font-size: 14px;");
                rowLayout->addWidget(errLbl);
            }
            chatMessagesLayout->addWidget(rowWidget);
        }
        
        chatMessagesLayout->addStretch(); 
        
        // Auto-scroll to bottom smoothly
        QTimer::singleShot(30, [=]() { 
            chatScrollArea->verticalScrollBar()->setValue(chatScrollArea->verticalScrollBar()->maximum()); 
        });
    };

    // Fetches session titles from DB and populates the sidebar
    auto loadSessionsToSidebar = [&]() {
        sessionList->clear();
        QSqlQuery query("SELECT id, title FROM sessions ORDER BY id DESC");
        while (query.next()) {
            QListWidgetItem *item = new QListWidgetItem(" " + secureObfuscate(query.value(1).toString(), false));
            item->setData(Qt::UserRole, query.value(0).toInt());
            sessionList->addItem(item);
        }
    };

    // Handle clicking a session from the sidebar
    QObject::connect(sessionList, &QListWidget::itemClicked, [&](QListWidgetItem *item) {
        currentSessionId = item->data(Qt::UserRole).toInt();
        chatHistory.clear();
        QSqlQuery q; 
        q.prepare("SELECT role, content, model_info FROM messages WHERE session_id = :sid ORDER BY id ASC");
        q.bindValue(":sid", currentSessionId);
        if (q.exec()) {
            while (q.next()) {
                QString role = q.value(0).toString();
                QString content = secureObfuscate(q.value(1).toString(), false);
                QString model = q.value(2).toString();
                
                if (role == "user") chatHistory.push_back({ChatMessage::User, content, ""});
                else chatHistory.push_back({ChatMessage::AI, content, model});
            }
        }
        deleteChatBtn->setEnabled(true);
        renderChatHistory();
    });

    QObject::connect(newChatBtn, &QPushButton::clicked, [&]() { 
        currentSessionId = -1; chatHistory.clear(); deleteChatBtn->setEnabled(false); renderChatHistory(); 
    });

    QObject::connect(deleteChatBtn, &QPushButton::clicked, [&]() {
        if (currentSessionId != -1) {
            QSqlQuery q; q.prepare("DELETE FROM messages WHERE session_id = :sid"); q.bindValue(":sid", currentSessionId); q.exec();
            q.prepare("DELETE FROM sessions WHERE id = :sid"); q.bindValue(":sid", currentSessionId); q.exec();
            
            currentSessionId = -1; chatHistory.clear(); deleteChatBtn->setEnabled(false);
            loadSessionsToSidebar(); renderChatHistory();
        }
    });

    // Add initial loading message
    chatHistory.push_back({ChatMessage::System, "System is initializing...\nStarting Zero Trust Secure Gateway, please wait.", ""});
    renderChatHistory();

    // Settings window logic
    QObject::connect(settingsBtn, &QPushButton::clicked, [&]() {
        QDialog dialog(&window);
        dialog.setWindowTitle("Gateway Settings & Model Manager");
        dialog.setFixedSize(550, 620);
        auto *l = new QVBoxLayout(&dialog);
        
        auto *keysGroup = new QGroupBox("Cloud Provider API Keys");
        auto *keysLayout = new QFormLayout(keysGroup);
        auto *oKey = new QLineEdit(getSetting("openai_api_key")); oKey->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        auto *aKey = new QLineEdit(getSetting("anthropic_api_key")); aKey->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        auto *gKey = new QLineEdit(getSetting("gemini_api_key")); gKey->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        keysLayout->addRow("OpenAI:", oKey); keysLayout->addRow("Anthropic:", aKey); keysLayout->addRow("Gemini:", gKey);
        l->addWidget(keysGroup);
        
        auto *localGroup = new QGroupBox("Local AI Router (Ollama)");
        auto *localLayout = new QVBoxLayout(localGroup);
        auto *localCombo = new QComboBox();
        QString currentLocal = getSetting("local_model");
        if (currentLocal.isEmpty()) currentLocal = "qwen2.5-coder:7b";
        localCombo->addItem(currentLocal);

        // Connect to local Ollama API to fetch available models
        auto refreshLocalModels = [&networkManager, localCombo, currentLocal]() {
            QNetworkRequest req(QUrl("http://127.0.0.1:11434/api/tags"));
            QNetworkReply *rep = networkManager.get(req);
            QObject::connect(rep, &QNetworkReply::finished, [rep, localCombo, currentLocal]() {
                if (rep->error() == QNetworkReply::NoError) {
                    QJsonDocument doc = QJsonDocument::fromJson(rep->readAll());
                    QJsonArray models = doc.object()["models"].toArray();
                    localCombo->clear();
                    for (int i = 0; i < models.size(); i++) localCombo->addItem(models[i].toObject()["name"].toString());
                    localCombo->setCurrentText(currentLocal);
                }
                rep->deleteLater();
            });
        };
        refreshLocalModels(); 
        
        localLayout->addWidget(new QLabel("Active Routing Engine:")); localLayout->addWidget(localCombo);
        l->addWidget(localGroup);

        auto *downloadGroup = new QGroupBox("Download New Local Model");
        auto *downloadLayout = new QVBoxLayout(downloadGroup);
        auto *pullRow = new QHBoxLayout();
        auto *pullInput = new QLineEdit(); pullInput->setPlaceholderText("e.g. mistral, llama3.2, gemma:2b");
        
        auto *pullBtn = new QPushButton("Pull Model"); pullBtn->setObjectName("primaryBtn");
        pullBtn->setFixedSize(120, 35);
        
        pullRow->addWidget(pullInput); pullRow->addWidget(pullBtn);
        
        auto *progressBar = new QProgressBar(); progressBar->setRange(0, 100); progressBar->hide();
        progressBar->setStyleSheet("QProgressBar { border: none; background-color: #27272a; border-radius: 5px; height: 10px; text-align: center; } QProgressBar::chunk { background-color: #3b82f6; border-radius: 5px; }");
        auto *statusLabel = new QLabel(""); statusLabel->setStyleSheet("color: #3b82f6; font-weight: bold; font-size: 12px;"); statusLabel->hide();

        downloadLayout->addLayout(pullRow); downloadLayout->addWidget(progressBar); downloadLayout->addWidget(statusLabel);
        l->addWidget(downloadGroup);

        QPointer<QProgressBar> safeProgress(progressBar); QPointer<QLabel> safeStatus(statusLabel); QPointer<QPushButton> safeBtn(pullBtn);

        QObject::connect(pullBtn, &QPushButton::clicked, [&networkManager, pullInput, safeBtn, safeProgress, safeStatus, refreshLocalModels]() {
            QString modelName = pullInput->text().trimmed();
            if (modelName.isEmpty()) return;
            if(safeBtn) safeBtn->setDisabled(true); if(safeProgress) safeProgress->show(); if(safeStatus) { safeStatus->show(); safeStatus->setText("Contacting Ollama registry..."); }
            
            QJsonObject json; json["name"] = modelName;
            QNetworkRequest req(QUrl("http://127.0.0.1:11434/api/pull")); req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            QNetworkReply *rep = networkManager.post(req, QJsonDocument(json).toJson());
            
            QObject::connect(rep, &QNetworkReply::readyRead, [rep, safeProgress, safeStatus]() {
                QList<QByteArray> lines = rep->readAll().split('\n');
                for (const QByteArray& line : lines) {
                    if (line.trimmed().isEmpty()) continue;
                    QJsonObject obj = QJsonDocument::fromJson(line).object();
                    if(safeStatus) safeStatus->setText(obj["status"].toString());
                    if (obj.contains("completed") && obj.contains("total") && safeProgress) {
                        double completed = obj["completed"].toDouble(), total = obj["total"].toDouble();
                        if (total > 0) safeProgress->setValue(static_cast<int>((completed / total) * 100));
                    }
                }
            });
            QObject::connect(rep, &QNetworkReply::finished, [rep, safeBtn, safeProgress, safeStatus, modelName, pullInput, refreshLocalModels]() {
                if(safeBtn) safeBtn->setDisabled(false);
                if (rep->error() == QNetworkReply::NoError) {
                    if(safeStatus) safeStatus->setText("✅ Successfully installed: " + modelName);
                    if(safeProgress) safeProgress->setValue(100); if(pullInput) pullInput->clear();
                    refreshLocalModels(); 
                } else {
                    if(safeStatus) { safeStatus->setStyleSheet("color: #EF4444;"); safeStatus->setText("❌ Failed: " + rep->errorString()); }
                    if(safeProgress) safeProgress->hide();
                }
                rep->deleteLater();
            });
        });

        auto *saveBtn = new QPushButton("Save Preferences"); saveBtn->setObjectName("primaryBtn"); saveBtn->setFixedHeight(45);
        l->addSpacing(10); l->addWidget(saveBtn);
        
        QObject::connect(saveBtn, &QPushButton::clicked, [&]() {
            setSetting("openai_api_key", oKey->text().trimmed()); 
            setSetting("anthropic_api_key", aKey->text().trimmed());
            setSetting("gemini_api_key", gKey->text().trimmed()); 
            setSetting("local_model", localCombo->currentText());
            dialog.accept(); 
        });
        dialog.exec();
    });

    // Message transmission logic
    auto sendMessage = [&]() {
        QString text = inputField->toPlainText().trimmed();
        if (text.isEmpty()) return;

        if (text.length() > 100000) {
            chatHistory.push_back({ChatMessage::Error, "Security Block: Payload too large. Limit is 100,000 characters.", ""});
            renderChatHistory();
            return;
        }

        QString prov = providerCombo->currentText();
        QString mName = modelCombo->currentText();
        setSetting("last_provider", prov);

        QString apiKey = getSetting(prov + "_api_key");
        if (apiKey.isEmpty()) { 
            chatHistory.push_back({ChatMessage::Error, "API Key for " + prov.toUpper() + " is missing. Please add it in Settings.", ""}); 
            renderChatHistory(); 
            return; 
        }
        
        bool isIncognito = incognitoCheck->isChecked();
        
        // Save to DB if not incognito
        if (!isIncognito && currentSessionId == -1) {
            QSqlQuery q; q.prepare("INSERT INTO sessions (title) VALUES (:t)");
            q.bindValue(":t", secureObfuscate(text.left(30) + "...", true));
            if (q.exec()) { currentSessionId = q.lastInsertId().toInt(); loadSessionsToSidebar(); deleteChatBtn->setEnabled(true); }
        }
        if (!isIncognito && currentSessionId != -1) {
            QSqlQuery q; q.prepare("INSERT INTO messages (session_id, role, content, model_info) VALUES (:s, :r, :c, :m)");
            q.bindValue(":s", currentSessionId); q.bindValue(":r", "user"); q.bindValue(":c", secureObfuscate(text, true)); q.bindValue(":m", ""); q.exec();
        }

        chatHistory.push_back({ChatMessage::User, text, ""}); 
        renderChatHistory();
        
        inputField->clear(); 
        inputField->setDisabled(true); 
        sendButton->hide(); 
        cancelButton->show();

        // Build chat history array for API request
        QJsonArray historyArray;
        for (int i = std::max(0, (int)chatHistory.size() - 7); i < chatHistory.size() - 1; ++i) {
            if (chatHistory[i].type == ChatMessage::User || chatHistory[i].type == ChatMessage::AI) {
                QJsonObject msgObj;
                msgObj["role"] = (chatHistory[i].type == ChatMessage::User) ? "user" : "assistant"; 
                msgObj["content"] = chatHistory[i].content;
                historyArray.append(msgObj);
            }
        }

        QJsonObject json; 
        json["prompt"] = text; 
        json["provider"] = prov; 
        json["model_name"] = mName;
        QString localMod = getSetting("local_model"); 
        if(localMod.isEmpty()) localMod = "qwen2.5-coder:7b";
        json["local_model"] = localMod; 
        json["history"] = historyArray; 

        // Send request to localhost Python backend
        QUrl url("https://127.0.0.1:8000/api/v1/chat");
        QNetworkRequest request(url); 
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("x-api-key", apiKey.toUtf8()); 
        request.setRawHeader("x-provider", prov.toUtf8());
        
        activeReply = networkManager.post(request, QJsonDocument(json).toJson());
        
        // Ignore SSL errors since we generate self-signed certs dynamically
        QObject::connect(activeReply, &QNetworkReply::sslErrors, [activeReply](const QList<QSslError> &errors) { 
            activeReply->ignoreSslErrors(errors); 
        });

        QObject::connect(activeReply, &QNetworkReply::finished, [&]() {
            inputField->setDisabled(false); cancelButton->hide(); sendButton->show(); inputField->setFocus();
            if (activeReply->error() == QNetworkReply::NoError) {
                QJsonObject res = QJsonDocument::fromJson(activeReply->readAll()).object();
                QString aiMsg = res["llm_response"].toString();
                QString mUsed = res["model_used"].toString();
                
                chatHistory.push_back({ChatMessage::System, "DLP_REPORT|" + res["sanitized_prompt"].toString(), ""});
                chatHistory.push_back({ChatMessage::AI, aiMsg, mUsed});
                
                if (!isIncognito && currentSessionId != -1) {
                    QSqlQuery q; q.prepare("INSERT INTO messages (session_id, role, content, model_info) VALUES (:s, :r, :c, :m)");
                    q.bindValue(":s", currentSessionId); q.bindValue(":r", "ai"); q.bindValue(":c", secureObfuscate(aiMsg, true)); q.bindValue(":m", mUsed); q.exec();
                }
            } else { 
                QByteArray errData = activeReply->readAll(); 
                QString errText = activeReply->errorString();
                if (!errData.isEmpty()) {
                    QJsonDocument errDoc = QJsonDocument::fromJson(errData);
                    if (errDoc.isObject() && errDoc.object().contains("detail")) errText = errDoc.object()["detail"].toString();
                }
                chatHistory.push_back({ChatMessage::Error, errText, ""}); 
            }
            renderChatHistory(); activeReply->deleteLater();
        });
    };

    QObject::connect(cancelButton, &QPushButton::clicked, [&]() { 
        if (activeReply && activeReply->isRunning()) activeReply->abort(); 
    });
    
    auto *enterFilter = new EnterSubmitFilter(&window); 
    enterFilter->onSubmit = sendMessage; 
    inputField->installEventFilter(enterFilter);
    QObject::connect(sendButton, &QPushButton::clicked, sendMessage);
    
    // Launch the Python backend silently
    auto *backendProcess = new QProcess(&window);
    QString appDir = QCoreApplication::applicationDirPath();
    backendProcess->setWorkingDirectory(appDir); 
    QString backendExePath = appDir + "/backend_engine/main.exe";
    backendProcess->start(backendExePath, {});

    auto isBackendReady = std::make_shared<bool>(false);
    QObject::connect(backendProcess, &QProcess::readyReadStandardError, [&, isBackendReady]() {
        QString log = QString::fromUtf8(backendProcess->readAllStandardError());
        if(!*isBackendReady) {
            if(log.contains("Uvicorn running on") || log.contains("Application startup complete")) {
                *isBackendReady = true;
                inputField->setDisabled(false); sendButton->setDisabled(false);
                inputField->setPlaceholderText("Message Zero Trust AI...");
                chatHistory.clear(); 
                chatHistory.push_back({ChatMessage::System, "SECURE GATEWAY READY", ""});
                renderChatHistory(); 
            } 
            else if (log.contains("Error", Qt::CaseInsensitive) || log.contains("Errno") || log.contains("Failed") || log.contains("Exception")) {
                chatHistory.clear();
                chatHistory.push_back({ChatMessage::Error, "Failed to start Gateway:\n" + log.trimmed(), ""});
                inputField->setPlaceholderText("System crashed, please check the logs.");
                renderChatHistory();
            }
        }
    });

    QObject::connect(themeCombo, &QComboBox::currentTextChanged, [&](const QString &t) { 
        applyModernTheme(app, t == "Dark Mode"); 
        renderChatHistory(); 
    });
    
    applyModernTheme(app, true); 
    loadSessionsToSidebar();

    window.show();
    return app.exec();
}
