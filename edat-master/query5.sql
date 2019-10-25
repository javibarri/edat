select s1.name
from
	(select language.name, count(language.name) as nums
	from language, film, rental, inventory
	where language.language_id = film.language_id and 
		film.film_id = inventory.inventory_id and
		inventory.inventory_id = rental.inventory_id
	group by language.name) as s1,
	
	(select language.name, count(language.name) as nums
	from language, film, rental, inventory
	where language.language_id = film.language_id and 
		film.film_id = inventory.inventory_id and
		inventory.inventory_id = rental.inventory_id
	group by language.name
	limit 1) as s2

where s1.nums = s2.nums

	
	
 