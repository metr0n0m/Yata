# План: подсветка ключевых слов/фраз в Yata

## Цель
Пользователь задаёт список ключевых слов или фраз (например `access deny`,
`fatal error`), для каждого выбирает цвет, и в потоке лога совпадения
отображаются подсвеченными. Работает и при обычном просмотре, и в режиме tail.

## Подход: QSyntaxHighlighter (без проходов по документу)
Используется штатный механизм Qt — `QSyntaxHighlighter`. Он вызывается Qt
**поблочно по событию**, только для изменившихся строк. Никаких циклов по
документу, никакого повторного чтения файла, никакого второго прохода.

Единственный рабочий метод:
```cpp
void highlightBlock(const QString &text) override {
    // text — одна строка, которую Qt сейчас раскладывает.
    // Для каждого правила: найти совпадение и setFormat(pos, len, format).
}
```

В режиме больших файлов (`PartialLayout`) в `QTextDocument` и так лежит только
видимый экран (`PartialLayout.cpp:250-254`, грузится `numLinesOnScreen()+1`
строк), поэтому подсветка касается только видимых строк вне зависимости от
размера лога и скорости tail.

## Шаги реализации

### 1. Модель данных правила подсветки
- Новый класс `HighlightRule { QString pattern; bool isRegex; TextColor color; }`
  (цвет — через уже существующий класс `preferences/TextColor`).
- Хранить список правил в `Preferences` рядом с `m_normalTextColor` /
  `m_selectedTextColor` (`preferences/Preferences.h`).

### 2. Класс подсветки
- Новый `LogHighlighter : public QSyntaxHighlighter`.
- Конструктор принимает `QTextDocument*` (у `YTextDocument` он есть —
  `m_document`).
- В `highlightBlock()` перебрать правила, найти совпадения (`QString::indexOf`
  для фраз или `QRegExp`/`QRegularExpression` для регексов) и вызвать
  `setFormat()` на диапазон.
- Создать экземпляр в `YTextDocument` и привязать к его `m_document`.

### 3. Совместимость с выделением мышью
- Подсветка ключевых слов и «выделение» (`YTextDocument::select`) — разные
  слои форматирования. Убедиться, что цвет выделения виден поверх подсветки
  (проверить порядок применения форматов; `QSyntaxHighlighter` работает поверх
  базового формата блока, выделение накладывается отдельно — проверить на
  практике).

### 4. UI редактора правил
- В `PreferencesDialog` (`gui/PreferencesDialog.*`, форма
  `gui/PreferencesDialog.ui`) добавить секцию: список правил, кнопки
  «Добавить/Удалить», поле ввода фразы, флажок «регулярное выражение» и
  `ColorButton` (класс уже есть: `gui/ColorButton`).
- Переиспользовать паттерн из `TextColorWidget`.

### 5. Сохранение/загрузка (YAML)
- Дописать секцию `highlights` в `Preferences::write()` и `Preferences::read()`
  (`preferences/Preferences.cpp`) — по образцу уже существующей сериализации
  цветов через yaml-cpp.

### 6. Обновление в реальном времени (tail)
- Отдельных действий почти не требуется: при дозаписи файла
  `TailView::onFileChanged()` перезапускает layout, `QSyntaxHighlighter`
  автоматически переподсветит изменившиеся/новые блоки.
- При изменении правил в настройках — вызвать `rehighlight()` на активных
  подсветчиках (можно повесить на сигнал `Preferences::preferencesChanged()`,
  который уже слушается в `TailView::onPreferencesChanged`).

## Оценка сложности
Средняя, ближе к несложной. Основная работа — UI редактора списка и
сериализация. Сам движок подсветки — небольшой класс поверх штатного
`QSyntaxHighlighter`.

## Риски / нюансы
- **Кодировка**: для латиницы (`access deny`, `fatal error`) — без проблем. Для
  кириллицы упрётся в текущее побайтное чтение как Latin-1 (`fileio/`), это
  отдельная проблема из общего аудита, не блокер для латинских фраз.
- **Порядок форматов** с выделением мышью — проверить на практике (шаг 3).
- Фича аддитивная: не трогает чтение файла, скролл, поиск, слежение за хвостом,
  поэтому риск сломать рабочую программу низкий.

---

# Что сам автор пометил как недоделанное / в планах

Из файлов документации и комментариев в коде (не мои выводы — прямые пометки
автора):

## Из документации
- **`readme.txt:59`** — «This software is *alpha* quality software, and is not
  intended for general use yet» (общий статус: альфа).
- **`changes.txt:5`** — «Rudimentary text selection and text copying»
  (выделение и копирование текста названы *зачаточными* / недоделанными самим
  автором в списке изменений 0.2.0).

## TODO-комментарии в коде
- **`view/TailView.cpp:357`** — `// TODO: implement double click selection`
  (выделение по двойному клику не реализовано — метод `mouseDoubleClickEvent`
  пустой).
- **`preferences/Preferences.cpp:20`** — `// TODO: implement versioning for
  preference files` (нет версионирования файла настроек).
- **`preferences/Preferences.cpp:82`** — `// TODO: handle parsing errors` (ошибки
  разбора файла настроек не обрабатываются — просто игнорируются).
- **`view/PartialLayout.cpp:36`** — `// TODO: scrollbar didn't move, but
  updateView() assumes it did...` (известная нестыковка логики скроллбара при
  follow-tail).
- **`document/YTextDocument.h:71`** — `// TODO: make yFileCursor() private`
  (метод временно публичный, планировалось скрыть).
- **`gui/YTabWidget.cpp:19`** — `// TODO: icons` (иконки вкладок не сделаны).
- **`session/SessionCommon.h:4`** — `// TODO: move this to a yaml subdirectory...`
  (рефакторинг размещения YAML-хелперов).
- **`yata.pro:21`** — `# TODO: make the following line unnecessary` (костыль в
  сборочном файле с путями `INCLUDEPATH`).

## Наполовину реализованная функциональность (по коду, не по комментарию)
- **Сессии сохраняют не всё**: поля `FileSession.address` (позиция скролла) и
  `followTail` пишутся в формат (`session/FileSession.cpp`), но `MainWindow`
  их не заполняет и `SessionLoader` не восстанавливает — то есть при
  перезапуске позиция в файле и режим follow-tail не восстанавливаются.
