# CommentsListView
Creating a custom widget to display messages from an SqlModel inside a ListView.

For Editing the list's items, double click the rectangle you want to edit.
Immediatly after a editor will pop up and the comment's body is now editable. To confirm the editing, press "Ctrl+Return"
(for now the commitment to save the data to the model doesn't work -- see the WARNING paragraph)

![image](https://github.com/user-attachments/assets/d95a8674-d138-4c4d-9eaa-caf294b17b1b)

- WARNING:
- The SQl model is only a query filtered from the original table, present in the PostgressDatabase, so
- the QStyledItemList::setModelData() won't work

- The Problems are highlighted with a warning comment like so:
  ![image](https://github.com/user-attachments/assets/f8654a7d-4d94-4a37-b6ff-50ac33f35df5)

You can find them inside "commentlistitem.cpp" and inside "mainwindow.cpp"
