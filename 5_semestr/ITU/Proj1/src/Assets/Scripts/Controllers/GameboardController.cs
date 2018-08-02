using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameboardController : MonoBehaviour
{
    public GameObject UpperCard;
    public GameObject DownerCard;
    public GameObject PlayerInfos;
    public GameObject PopUp;
    private ShowForXSeconds Show;

    public List<GameObject> Figurines = new List<GameObject>();
	public List<GameObject> Fields = new List<GameObject>();
	public List<GameObject> PlayerCards = new List<GameObject>();

	private List<FieldController> FieldsControllers = new List<FieldController>(41);
	private bool endTurn;
	private int figureinplay;


	void Awake () {

		foreach (GameObject field in Fields) {
			FieldsControllers.Add ((FieldController)field.GetComponent (typeof(FieldController)));
		}

		endTurn = true;
		figureinplay = 0;

        this.Show = PopUp.GetComponent<ShowForXSeconds>();

	}
	
	// Update is called once per frame
	void Update () {
		
	}


	//activates pipeend
	public void ActivatePipeEnd(int FieldNumber, GameObject figure)
	{
		GameObject field = Fields [FieldNumber];

		Component[] pipeEnd = field.GetComponentsInChildren(typeof(Image), true);
	

		foreach(Image pipe in pipeEnd) {
			pipe.enabled = true;
		}
	}

	//deactivates pipeend
	public void DeactivatePipeEnd(int FieldNumber, GameObject figure)
	{
		GameObject field = Fields [FieldNumber];

		if (FieldsControllers [FieldNumber - 1].GetAmountOfFiguresInField() == 1) {

			Component[] pipeEnd = field.GetComponentsInChildren(typeof(Image), true);

			foreach(Image pipe in pipeEnd) {
				if (pipe.name.Equals("KonecTrubky")) {
					pipe.enabled = false;
				}

			}

		}

	}

	public void AddFiguretoFieldandChangeActiveCard (int FieldNumber, GameObject figure) 
	{
		if (FieldNumber != 0) {
			FieldsControllers[FieldNumber-1].AddFigureToField(figure);
		} else {
			FieldsControllers[FieldNumber].AddFigureToField(figure);
		}
	}

	public void RemoveFigureFromField (int FieldNumber) 
	{
		if (FieldNumber != 0) {
			FieldsControllers[FieldNumber-1].RemoveFigureFromField ();
		} else {
			FieldsControllers[FieldNumber].RemoveFigureFromField ();
		}
	}

	public void activatefigure()
	{

		if (endTurn == true) {

			if (figureinplay % 6 == 0) {
				figureinplay = 0;
			}

			endTurn = false;
		

			FigureController figureController = (FigureController)Figurines [figureinplay].GetComponent (typeof(FigureController));
			int hozeno = figureController.throwdice ();
            Show.makevisible(hozeno-1);


			figureinplay++;
		}
			
	}

	public void EndTurn() {

		endTurn = true;

	}


}
