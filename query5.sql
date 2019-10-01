select max(language.name)
from language, film, rental, inventory
where language.language_id = film.language_id and 
	film.film_id = inventory.inventory_id and
	inventory.inventory_id = rental.inventory_id

	
	
 