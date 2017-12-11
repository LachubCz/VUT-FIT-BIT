using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using UnityEngine.UI;
using System.Text.RegularExpressions;

public class FieldController : MonoBehaviour {
	

	public GameObject ActiveCard;
	public GameObject Field;


	private List<GameObject> FiguresInField;
	private HorseController horseController;
	private string FieldName;
	private string WhichWay;

	private bool moveFiguresInFieldAfterRemove;
	private bool moveFiguresInFieldAfterAdd;

	// Use this for initialization
	void Awake () {
		FiguresInField = new List<GameObject> (6);
		horseController = (HorseController) ActiveCard.GetComponent (typeof(HorseController));

		Text FieldText = (Text) (Field.GetComponentInChildren (typeof(UnityEngine.UI.Text)));
		FieldName = FieldText.text;

		moveFiguresInFieldAfterRemove = false;
		moveFiguresInFieldAfterAdd = false;



		string positionString = Regex.Match(Field.name.Substring (7, 2), @"\d+").Value;
		int position = Int32.Parse (positionString);

		if (position >= 1 && position <= 8 ||
		    position >= 17 && position <= 24 ||
		    position >= 33 && position <= 40) {
			WhichWay = "right";
		} else {
			WhichWay = "left";
		}
	}
	
	// Update is called once per frame
	void Update () {


		if (moveFiguresInFieldAfterRemove) {
			float move = 20f;

			foreach (GameObject figure in FiguresInField) {

				for (float o = 0; o <= move; o += 0.5f) {

					Vector3 pos = figure.transform.position;

					if (WhichWay == "right") {
						pos.x -= 0.5f;
						figure.transform.position = pos;
					} else {
						pos.x += 0.5f;
						figure.transform.position = pos;
					}


				}


			}

			moveFiguresInFieldAfterRemove = false;
		}

		if(moveFiguresInFieldAfterAdd) {


				float move = 0f;
				int i = 1;

				foreach (GameObject figure in FiguresInField) {

					for (float o = 0; o <= move; o += 0.5f) {

						if (i == FiguresInField.Count) {
							Vector3 pos = figure.transform.position;

							if (WhichWay == "right") {
								pos.x += 0.5f;
								figure.transform.position = pos;
							} else {
								pos.x -= 0.5f;
								figure.transform.position = pos;
							}
						}

					}


					i++;
					move += 20f;
				}



				moveFiguresInFieldAfterAdd = false;


		}

	}

	public void AddFigureToField(GameObject figure)
	{
		FiguresInField.Add (figure);
		horseController.changeViewCard (FieldName);

		moveFiguresInFieldAfterAdd_ ();
	}


	public void RemoveFigureFromField()
	{
		if (FiguresInField.Count != 0) 
		{
			FiguresInField.RemoveAt(0);
			moveFiguresInFieldAfterRemove_ ();
		}

	}


	private void moveFiguresInFieldAfterRemove_() 
	{
		moveFiguresInFieldAfterRemove = true;
	}

	private void moveFiguresInFieldAfterAdd_()
	{
		moveFiguresInFieldAfterAdd = true;

	}

	public int GetAmountOfFiguresInField()
	{
		return FiguresInField.Count;
	}
		
}
