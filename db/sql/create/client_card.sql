DROP TABLE client_card;

CREATE TABLE client_card
(
	"client_id" INTEGER NOT NULL,
	"card_id" INTEGER NOT NULL,
	"number" INTEGER NOT NULL
);

ALTER TABLE client_card
	ADD CONSTRAINT client_id_fk 
		FOREIGN KEY (client_id) REFERENCES client (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;
ALTER TABLE client_card
	ADD CONSTRAINT card_id_fk 
		FOREIGN KEY (card_id) REFERENCES card (id)
			MATCH SIMPLE 
			ON UPDATE CASCADE 
			ON DELETE CASCADE;