SELECT title, title_no FROM title

SELECT title FROM title
WHERE title_no=10

SELECT copy_no, fine_assessed FROM loanhist
WHERE fine_assessed BETWEEN 8 AND 9 

SELECT title_no, author FROM title
WHERE author IN ('Charles Dickens', 'Jane Austen')

SELECT title_no, title FROM title
WHERE title LIKE '%adventures%'

SELECT member_no, fine_assessed, fine_paid, fine_waived from loanhist
WHERE fine_assessed IS NOT NULL AND NOT (fine_paid=fine_waived)  ---> ?!?!

SELECT DISTINCT city, state FROM adult
