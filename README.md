# CommentsListView
![image](https://github.com/user-attachments/assets/74923797-a99a-4ca8-98c1-ba894362b1e7)

Creating a custom widget to display messages from an SqlModel inside a ListView.

For Editing the list's items, double click the rectangle you want to edit.
Immediatly after a editor will pop up and the comment's body is now editable. To confirm the editing, press "Ctrl+Return"


## key functionalities:
1) writing the VIN (vehicle ID number) on the LineEdit at the top and pressing "Return" will filter the view
2) Double click to edit the item's body (I added a restriction where only the username provided in the column 2 (USERNAME) can edit is own comments)
3) After the editor is closed (by pressing CTRL+Return) the model will update the "BODY_MESSAGE" column and automatically the Editing_date
4) -- coming soon -- : lineEdit to create a new Item to append in the listView


## Important SetUp
In order to connect correctly with the database you have to create a new "file.h" and fill in yours information like so:

"file.h"
```c++
namespace db_info
{
    const char HOST[] = "localhost";
    const int PORT{5432};
    const char CONNECTION_NAME[] = "postgres";
    const char USER_NAME[] = "postgres";
    const char DB_NAME[] = "comments_db";
    const char PASSWORD[] = "password_to_postgres_db";    //update this value with your password
}
```

that is used in MainWindow::setUpDbConnection()
```c++
void MainWindow::setUpDbConnection()
{
    // connecting to the postgres database
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(db_info::HOST);
    m_db.setDatabaseName(db_info::DB_NAME);
    m_db.setUserName(db_info::USER_NAME);
    m_db.setPassword(db_info::PASSWORD);
    m_db.setPort(db_info::PORT);

    if (!m_db.open()) {
        qDebug() << "Error: Could not open database";
    }
}
```

#--------- SQL CODE to crate DUMMY DB --------------

```SQL
-------------------------------------------------------------------
                        VIN TABLE
-------------------------------------------------------------------

CREATE TABLE VIN_table (
    id BIGSERIAL PRIMARY KEY,
    VIN VARCHAR(32) NOT NULL UNIQUE,
    PRJ_CODE VARCHAR(32),
    MAKE_YEAR DATE
);

/* RECORD 1 */ 
INSERT INTO VIN_table (VIN, PRJ_CODE, MAKE_YEAR)
VALUES ('5LMPU28A8YLJ84702', 'PRJ100', '2019/01/01'::DATE);

/* RECORD 2 */
INSERT INTO VIN_table (VIN, PRJ_CODE, MAKE_YEAR)
VALUES ('3NIAB51D95L401539', 'PRJ155', '2019/01/01'::DATE);

/* RECORD 3 */
INSERT INTO VIN_table (VIN, PRJ_CODE, MAKE_YEAR)
VALUES ('5N3AA08C14N800205', 'PRJ25', '2017/01/01'::DATE);

/* RECORD 4 */
INSERT INTO VIN_table (VIN, PRJ_CODE, MAKE_YEAR)
VALUES ('4A3AK34YIXE055700', 'PRJ75', '2010/01/01'::DATE);

/* RECORD 5 */
INSERT INTO VIN_table (VIN, PRJ_CODE, MAKE_YEAR)
VALUES ('2GIWF52E749130019', 'PRJ10', '2020/01/01'::DATE);

SELECT * FROM vin_table;

DROP TABLE vin_table;   /*  use only to clear the data and rebuilt from 0 */



-------------------------------------------------------------------
                        USERS TABLE
-------------------------------------------------------------------

CREATE TABLE USERS_table (
    id BIGSERIAL PRIMARY KEY,
    MAIL VARCHAR(100) NOT NULL UNIQUE,
    PASSWORD VARCHAR(100),
    PRIVILAGE VARCHAR(32) CHECK(PRIVILAGE IN ('ADMIN', 'USER'))
);

/* RECORD 1 */
INSERT INTO USERS_table (MAIL, PASSWORD, PRIVILAGE)
VALUES ('userl@gmail.com', '123password', 'USER');

/* RECORD 2 */
INSERT INTO USERS_table (MAIL, PASSWORD, PRIVILAGE)
VALUES ('123user@gmail.com', 'qwerty135', 'USER');

/* RECORD 3 */
INSERT INTO USERS_table (MAIL, PASSWORD, PRIVILAGE)
VALUES ('us.er@gmail.com', 'simplePassword', 'ADMIN');

/* RECORD 4 */
INSERT INTO USERS_table (MAIL, PASSWORD, PRIVILAGE)
VALUES ('random_user@gmail.com', 'pASSW0rd', 'ADMIN');


SELECT * FROM USERS_table;

DROP TABLE USERS_table;



-------------------------------------------------------------------
                        COMMENTS TABLE
-------------------------------------------------------------------

CREATE TABLE COMMENTS_table (
    id BIGSERIAL PRIMARY KEY,
    CHASSIS_ID BIGINT NOT NULL,
    USER_ID BIGINT NOT NULL,
    POST_DATE TIMESTAMP NOT NULL UNIQUE,
    POST_EDITING_DATE TIMESTAMP UNIQUE,
    BODY_MESSAGE TEXT NOT NULL,
    
    CONSTRAINT ext_key_chassis_id
        FOREIGN KEY(CHASSIS_ID)
        REFERENCES VIN_table(id),

    CONSTRAINT ext_key_user_id
        FOREIGN KEY(USER_ID)
        REFERENCES USERS_table(id)
);

/* RECORD 1 */
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, BODY_MESSAGE)
VALUES (3, 2, '2023/02/19 13:30:00'::timestamp, 'RANDOM TEXT 1');

/* RECORD 2 */
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, POST_EDITING_DATE, BODY_MESSAGE)
VALUES (3, 2, '2023/06/20 11:30:00'::timestamp, '2023/06/22 14:30:00'::timestamp, '123 TEXT TEXT');

/* RECORD 3 */ 
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, BODY_MESSAGE)
VALUES (2, 1, '2023/07/03 8:30:00'::timestamp, 'TEST MESSAGE');

/* RECORD 4 */
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, BODY_MESSAGE)
VALUES (3, 4, '2023/09/08 10:20:00'::timestamp, 'RND TEXT 2');

/* RECORD 5 */
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, POST_EDITING_DATE, BODY_MESSAGE)
VALUES (1, 3, '2024/12/24 18:30:00'::timestamp, '2025/01/05 11:55:00'::timestamp, 'TXT TXT TXT');

/* RECORD 6 */
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, BODY_MESSAGE)
VALUES (4, 1, '2025/01/01 13:30:00'::timestamp, 'MY TEXT');

/* RECORD 7 */ 
INSERT INTO COMMENTS_table (CHASSIS_ID, USER_ID, POST_DATE, BODY_MESSAGE)
VALUES (5, 2, '2025/02/19 17:58:00'::timestamp, 'NO TEXT HERE');


SELECT * FROM COMMENTS_table;

DROP TABLE COMMENTS_table;
```

