SELECT UnitPrice*Quantity*(1-Discount) as 'total_value' FROM [Order Details]
ORDER BY total_value DESC

SELECT TOP 10 UnitPrice*Quantity*(1-Discount) as 'total_value' 
FROM [Order Details]
ORDER BY total_value DESC

SELECT TOP 10 WITH TIES UnitPrice*Quantity*(1-Discount) as 'total_value' 
FROM [Order Details]
ORDER BY total_value DESC

SELECT SUM(Quantity) as 'total_quantity' FROM [Order Details]
WHERE ProductID < 3

SELECT ProductID, SUM(Quantity) as 'total_quantity' FROM [Order Details]
GROUP BY ProductID
ORDER BY ProductID

SELECT OrderID, SUM(UnitPrice * Quantity * (1-Discount)) as 'total_quantity' FROM [Order Details]
GROUP BY OrderID
HAVING SUM(Quantity) > 250

SELECT ProductID, OrderID, SUM(Quantity) as 'total_quantity' FROM [Order Details]
GROUP BY ProductID, OrderID
WITH ROLLUP

SELECT ProductID, OrderID, SUM(Quantity) as 'total_quantity' FROM [Order Details]
WHERE ProductID = 50
GROUP BY ProductID, OrderID
WITH ROLLUP

SELECT ProductID, GROUPING(ProductID),OrderID, GROUPING(OrderID), SUM(Quantity) as 'total_quantity' FROM [Order Details]
GROUP BY ProductID, OrderID
WITH CUBE
