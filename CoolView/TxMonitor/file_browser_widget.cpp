#include "file_browser_widget.h"

#include <QFileSystemModel> 

#include "util\ini\TxConfig.h"
#include "util.h"


FileBrowserWidget::FileBrowserWidget(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);
}

FileBrowserWidget::~FileBrowserWidget()
{

}

void FileBrowserWidget::Initialize()
{
  QFileSystemModel *file_system_model = new QFileSystemModel;
  file_system_model->setReadOnly(true);

  ui.fileView->setModel(file_system_model);
  //ui.fileView->header()->setSortIndicator(0, Qt::AscendingOrder);
  //ui.fileView->header()->setSortIndicatorShown(true);
  //ui.fileView->header()->setSectionsClickable(true);
  ui.fileView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui.fileView->setSelectionBehavior(QAbstractItemView::SelectRows);

  //将根目录设为录像文件存放位置，防止用户乱看
  library_path_ = GetAbsolutePath(
    QString::fromLocal8Bit(CTxConfig::getInstance().GetUploadPath()));
  MakeDirRecursive(qPrintable(library_path_)); //确保路径有效
  ui.fileView->setRootIndex(file_system_model->setRootPath(library_path_));

  connect(ui.fileView, &QTreeView::expanded,
    this, &FileBrowserWidget::HandleDirectoryExpandedSlot);
}

void FileBrowserWidget::ExpandToPath( 
  const QString &file )
{
  QFileSystemModel *file_system_model = dynamic_cast<QFileSystemModel *>(ui.fileView->model());
  if (!file_system_model) {
    return;
  }
  QModelIndex index = file_system_model->index(file);
  if (index.isValid()) {
    if (index.parent().isValid() && 
        ui.fileView->isExpanded(index.parent())) {
      ui.fileView->scrollTo(index); //确保可见再选中
      ui.fileView->selectionModel()->select(index, 
        QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    } else {
      //scrollTo是从上级目录开始到根目录逐级展开，如果是首次展开，则QFileSystemModel还未加载目录，
      //会导致选中无效，所以我们要自己从根目录开始逐级展开……
      //但是首次加载目录还是有scrollTo无效的问题
      //ui.fileView->scrollTo(index);
      //记录以便文件夹展开后选中
      index_to_select_ = index;
      QStack<QModelIndex> stack;
      while (index.isValid()) {
        stack.push(index);
        index = index.parent();
      }
      while (!stack.isEmpty()) {
        ui.fileView->expand(stack.pop());
      }
    }
  }
}

void FileBrowserWidget::HandleDirectoryExpandedSlot(const QModelIndex & index)
{
  if (index_to_select_.isValid() && index_to_select_.parent() == index) {
    ui.fileView->selectionModel()->select(index_to_select_, 
      QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    ui.fileView->scrollTo(index_to_select_);
    index_to_select_ = QModelIndex();
  }
}
