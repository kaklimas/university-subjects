SELECT title FROM title
ORDER BY title

SELECT member_no, isbn, fine_assessed, fine_assessed * 2 AS 'double_fine' 
FROM loanhist
WHERE GETDATE() > out_date AND fine_assessed > 0 AND fine_assessed IS NOT NULL

SELECT LOWER(firstname + middleinitial + SUBSTRING(lastname, 0, 2)) AS 'email_name' 
FROM member
WHERE lastname='Anderson'

SELECT 
'The title is: ' + title + ', title number ' + 
CONVERT(varchar, title_no) AS 'new_title' 
FROM title
